/**
 * @file        sakura_root.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "sakura_root.h"
#include <tree_handler.h>

#include <processing/common/item_methods.h>
#include <processing/sakura_thread.h>
#include <processing/sakura_tree_callbacks.h>
#include <processing/thread_pool.h>

#include <libKitsunemimiSakuraNetwork/sakura_network.h>
#include <libKitsunemimiSakuraParser/sakura_parsing.h>
#include <libKitsunemimiJinja2/jinja2_converter.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiPersistence/logger/logger.h>

#include <items/sakura_items.h>
#include <converter/converter.h>

namespace SakuraTree
{

SakuraRoot* SakuraRoot::m_root = nullptr;
std::string SakuraRoot::m_executablePath = "";
TableItem SakuraRoot::m_errorOutput;
Jinja2Converter* SakuraRoot::m_jinja2Converter = nullptr;
TreeHandler* SakuraRoot::m_treeHandler = nullptr;
Kitsunemimi::Sakura::SakuraNetwork* SakuraRoot::m_networking = nullptr;

/**
 * @brief constructor
 *
 * @param executablePath path of the current executed SakuraTree-binary
 */
SakuraRoot::SakuraRoot(const std::string &executablePath,
                       const bool enableDebug)
{
    m_enableDebug = enableDebug;

    // initialzed static variables
    m_root = this;
    m_executablePath = executablePath;
    m_jinja2Converter = new Jinja2Converter();
    m_treeHandler = new TreeHandler();

    // initialize error-output
    m_errorOutput.addColumn("Field");
    m_errorOutput.addColumn("Value");

    // initialize thread-pool
    // TODO: make the number of initialized threads configurable
    m_threadPool = new ThreadPool(8);

    m_networking = new Kitsunemimi::Sakura::SakuraNetwork(this,
                                                          &sessionCallback,
                                                          &treeTransferCallback,
                                                          &seedTriggerCallback,
                                                          &blossomOutputCallback);
}

/**
 * destructor
 */
SakuraRoot::~SakuraRoot()
{
    // TODO: clear thread-pool and subtree-queue
    if(m_rootThread != nullptr)
    {
        m_rootThread->stopThread();
        delete m_rootThread;
        m_rootThread = nullptr;
    }
}

/**
 * initialize and start rollout-process
 *
 * @param initialTreePath
 * @param seedPath
 * @param initialValues
 * @param serverAddress
 * @param port
 *
 * @return true if successful, else false
 */
bool
SakuraRoot::startProcess(const std::string &inputPath,
                         const std::string &seedPath,
                         const DataMap &initialValues,
                         const std::string &serverAddress,
                         const uint16_t serverPort,
                         const std::string &initialTreeId)
{
    // load predefined trees
    if(m_treeHandler->loadPredefinedSubtrees() == false)
    {
        LOG_ERROR("failed to loag predefined trees");
        return false;
    }

    // start server
    if(serverAddress != ""
            && serverPort != 0)
    {
        if(m_networking->createServer(serverPort) == false)
        {
            LOG_ERROR("failed to create server on port " + std::to_string(serverPort));
            return false;
        }
    }

    // process seed
    if(seedPath != "")
    {
        SakuraItem* seedItem = prepareSeed(seedPath,
                                           m_executablePath,
                                           serverAddress,
                                           serverPort);
        if(seedItem == nullptr)
        {
            LOG_ERROR("failed to loag predefined trees");
            return false;
        }

        if(runProcess(seedItem, initialValues) == false) {
            return false;
        }

        // wait until all hosts ready
        uint32_t maxTries = 100;
        while(maxTries > 0)
        {
            if(m_networking->areAllHostsReady() == true) {
                break;
            }
            usleep(100000);
            maxTries--;
        }
        if(maxTries == 0) {
            return false;
        }
    }

    // process real task
    if(m_treeHandler->addTree(inputPath) == false) {
        return false;
    }

    shareAllTrees();

    SakuraItem* tree = m_treeHandler->getConvertedTree(initialTreeId);
    if(tree == nullptr) {
        return false;
    }
    if(runProcess(tree, initialValues) == false) {
        return false;
    }

    std::cout<<"finish"<<std::endl;

    return true;
}

/**
 * @brief SakuraRoot::startSubtreeProcess
 * @param treeId
 * @param values
 * @return
 */
bool
SakuraRoot::startSubtreeProcess(const std::string &treeId,
                                const std::string &values,
                                Kitsunemimi::Project::Session* session,
                                const uint64_t blockerId)
{
    std::cout<<"startSubtreeProcess"<<std::endl;

    // get tree
    SakuraItem* processPlan = m_treeHandler->getConvertedTree(treeId);
    if(processPlan == nullptr) {
        return false;
    }

    // run process
    JsonItem valuesJson;
    std::string errorMessage = "";
    valuesJson.parse(values, errorMessage);

    // run process by adding the tree as subtree-object to the subtree-queue to be processed by
    // one of the worker-threads
    SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
    object->subtree = processPlan;
    object->items = *valuesJson.getItemContent()->copy()->toMap();
    object->activeCounter = new SubtreeQueue::ActiveCounter();
    object->activeCounter->shouldCount = 1;
    object->session = session;
    object->blockerId = blockerId;
    m_threadPool->m_queue.addSubtreeObject(object);

    return true;
}

/**
 * @brief create an error-output
 *
 * @param blossomItem blossom-item with information of the error-location
 * @param errorLocation location where the error appeared
 * @param errorMessage message to describe, what was wrong
 * @param possibleSolution message with a possible solution to solve the problem
 */
void
SakuraRoot::createError(const BlossomItem &blossomItem,
                        const std::string &errorLocation,
                        const std::string &errorMessage,
                        const std::string &possibleSolution)
{
    createError(errorLocation,
                errorMessage,
                possibleSolution,
                blossomItem.blossomType,
                blossomItem.blossomGroupType,
                blossomItem.blossomName,
                blossomItem.blossomPath);
}

/**
 * @brief create an error-output
 *
 * @param errorLocation location where the error appeared
 * @param errorMessage message to describe, what was wrong
 * @param possibleSolution message with a possible solution to solve the problem
 * @param blossomType type of the blossom, where the error appeared
 * @param blossomGroup type of the blossom-group, where the error appeared
 * @param blossomName name of the blossom in the script to specify the location
 * @param blossomFilePath file-path, where the error had appeared
 */
void
SakuraRoot::createError(const std::string &errorLocation,
                        const std::string &errorMessage,
                        const std::string &possibleSolution,
                        const std::string &blossomType,
                        const std::string &blossomGroupType,
                        const std::string &blossomName,
                        const std::string &blossomFilePath)
{
    SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"ERROR", ""});
    if(errorLocation.size() > 0) {
        SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"location", errorLocation});
    }
    SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"error-message", errorMessage});

    if(possibleSolution.size() > 0)
    {
        SakuraRoot::m_errorOutput.addRow(
                    std::vector<std::string>{"possible solution",
                                             possibleSolution});
    }
    if(blossomType.size() > 0)
    {
        SakuraRoot::m_errorOutput.addRow(
                    std::vector<std::string>{"blossom-type",
                                             blossomType});
    }
    if(blossomGroupType.size() > 0)
    {
        SakuraRoot::m_errorOutput.addRow(
                    std::vector<std::string>{"blossom-group-type",
                                             blossomGroupType});
    }
    if(blossomName.size() > 0)
    {
        SakuraRoot::m_errorOutput.addRow(
                    std::vector<std::string>{"blossom-name",
                                             blossomName});
    }
    if(blossomFilePath.size() > 0)
    {
        SakuraRoot::m_errorOutput.addRow(
                    std::vector<std::string>{"blossom-file-path",
                                             blossomFilePath});
    }
    SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"", ""});
}

/**
 * @brief SakuraRoot::sendPlan
 * @param address
 * @param subtree
 * @param values
 * @return
 */
bool
SakuraRoot::sendTreefile(const std::string &address,
                         const std::string &subtree,
                         const std::string &values)
{
    return m_networking->sendTreePlanToHost(address, subtree, values);
}

/**
 * @brief SakuraRoot::startClientConnection
 * @param address
 * @param port
 * @return
 */
bool
SakuraRoot::startClientConnection(const std::string &address,
                                  const int port)
{
    return m_networking->createClientConnection(address, static_cast<uint16_t>(port));
}

/**
 * @brief SakuraRoot::printOutput
 * @param blossomItem
 */
void
SakuraRoot::printOutput(const BlossomItem &blossomItem)
{
    m_mutex.lock();

    std::cout<<" "<<std::endl;
    const std::string output = convertBlossomOutput(blossomItem);

    // only for prototyping hardcoded
    //m_networking->sendBlossomOuput("127.0.0.1", "", output);
    std::cout<<output<<std::endl;

    m_mutex.unlock();
}

/**
 * @brief print output-string
 *
 * @param output string, which should be printed
 */
void
SakuraRoot::printOutput(const std::string &output)
{
    // TODO: use logger instead
    m_mutex.lock();
    std::cout<<output<<std::endl;
    m_mutex.unlock();
}

/**
 * @brief SakuraRoot::runProcess
 * @return
 */
bool
SakuraRoot::runProcess(SakuraItem* item,
                       const DataMap &initialValues)
{
    // run process by adding the tree as subtree-object to the subtree-queue to be processed by
    // one of the worker-threads
    SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
    object->subtree = item;
    object->items = initialValues;
    object->activeCounter = new SubtreeQueue::ActiveCounter();
    object->activeCounter->shouldCount = 1;
    m_threadPool->m_queue.addSubtreeObject(object);

    // wait until the created subtree was fully processed by the worker-threads
    while(object->activeCounter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    // error-output
    // TODO: better solution necessary instead of checking the number of rows
    if(m_errorOutput.getNumberOfRows() > 0) {
        return false;
    }

    return true;
}

/**
 * @brief SakuraRoot::prepareSeed
 * @param seedPath
 * @param serverAddress
 * @param port
 * @return
 */
SakuraItem*
SakuraRoot::prepareSeed(const std::string &seedPath,
                        const std::string &executablePath,
                        const std::string &serverAddress,
                        const uint16_t port)
{
    Kitsunemimi::Sakura::SakuraParsing sakuraParsing(m_enableDebug);
    Converter converter;

    // parse seed-file
    const bool seedParseResult = sakuraParsing.parseFiles(seedPath);
    if(seedParseResult == false)
    {
        LOG_ERROR(sakuraParsing.getError().toString());;
        return nullptr;
    }

    // convert seed
    const JsonItem seed = sakuraParsing.getParsedFileContent();
    SakuraItem* convertedSeed = converter.convert(seed, true);
    if(convertedSeed == nullptr)
    {
        //std::cout<<m_errorOutput.toString()<<std::endl;
        LOG_ERROR(m_errorOutput.toString());
        return nullptr;
    }

    convertedSeed->values.insert("source_path", new DataValue(executablePath));
    convertedSeed->values.insert("server_address", new DataValue(serverAddress));
    convertedSeed->values.insert("server_port", new DataValue(port));

    return convertedSeed;
}

/**
 * @brief SakuraRoot::shareAllTrees
 */
void
SakuraRoot::shareAllTrees()
{
    std::map<std::string, TreeHandler::TreeHandlerItem>::const_iterator it;
    for(it = m_treeHandler->m_trees.begin();
        it != m_treeHandler->m_trees.end();
        it++)
    {
        m_networking->sendTreePlanToAll(it->first, it->second.parsedItem.toString());
    }
}

}
