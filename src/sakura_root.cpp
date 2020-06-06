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
#include <libKitsunemimiSakuraParser/sakura_items.h>

#include <libKitsunemimiJinja2/jinja2_converter.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiConfig/config_handler.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

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
 * @brief SakuraRoot::startProcess
 * @param configFilePath
 * @return
 */
bool
SakuraRoot::startProcess(const std::string &configFilePath)
{
    Kitsunemimi::Config::ConfigHandler configHandler;
    bool success = configHandler.initConfig(configFilePath);
    if(success == false) {
        return false;
    }

    return true;
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
                         const std::string &initialTreeId)
{
    // load predefined trees
    if(m_treeHandler->loadPredefinedSubtrees() == false)
    {
        LOG_ERROR("failed to load predefined trees");
        return false;
    }

    // process seed
    if(seedPath != "")
    {
        // start server
        if(m_networking->createServer(1337) == false)
        {
            LOG_ERROR("failed to create server on port " + std::to_string(1337));
            return false;
        }

        if(processSeed(seedPath) == false)
        {
            LOG_ERROR("failed process seed");
            return false;
        }
    }

    // process real task
    if(m_treeHandler->addTree(inputPath) == false)
    {
        LOG_ERROR(m_errorOutput.toString());
        return false;
    }

    SakuraItem* tree = nullptr;

    if(Kitsunemimi::Persistence::isFile(inputPath))
    {
        std::vector<std::string> splittedParts;
        Kitsunemimi::splitStringByDelimiter(splittedParts, inputPath, '/');
        const std::string rootPath = Kitsunemimi::Persistence::getParent(inputPath);
        const std::string relativePath = splittedParts.at(splittedParts.size() - 1);
        tree = m_treeHandler->getConvertedTree(rootPath, relativePath, initialTreeId);
    }
    else
    {
        tree = m_treeHandler->getConvertedTree(inputPath, "", initialTreeId);
    }

    if(tree == nullptr)
    {
        std::string errorMessage = "No tree found for the input-path " + inputPath;
        if(initialTreeId != "") {
            errorMessage += " and initial tree id : \"" + initialTreeId + "\"";
        }
        LOG_ERROR(errorMessage);
        return false;
    }

    if(runProcess(tree, initialValues) == false)
    {
        LOG_ERROR(m_errorOutput.toString());
        return false;
    }

    LOG_INFO("finish");

    return true;
}

/**
 * @brief SakuraRoot::startSubtreeProcess
 * @param treeId
 * @param values
 * @return
 */
bool
SakuraRoot::startSubtreeProcess(const std::string &relativePath,
                                const std::string &values,
                                Kitsunemimi::Project::Session* session,
                                const uint64_t blockerId)
{
    std::cout<<"startSubtreeProcess"<<std::endl;

    // get tree
    SakuraItem* processPlan = m_treeHandler->getConvertedTree("", relativePath);
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
 * @brief SakuraRoot::processSeed
 * @param seedPath
 * @return
 */
bool
SakuraRoot::processSeed(const std::string &seedPath)
{
    SeedItem* seedItem = prepareSeed(seedPath);

    if(seedItem == nullptr)
    {
        LOG_ERROR("failed to load seed-file " + seedPath);
        return false;
    }

    for(SeedPart* part : seedItem->childs)
    {
        std::vector<std::string> tags;

        // get and convert tags inside the part
        DataArray* unconvertedTags = dynamic_cast<DataArray*>(part->values.get("tags"));
        if(unconvertedTags != nullptr)
        {
            for(uint32_t i = 0; i < unconvertedTags->size(); i++)
            {
                tags.push_back(unconvertedTags->get(i)->toString());
            }
        }

        // register host based on the information
        m_networking->registerHost(part->id, tags);
    }

    // wait until all hosts ready or until timeout after 10 seconds
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

    shareAllTrees();

    return true;
}

/**
 * @brief SakuraRoot::prepareSeed
 * @param seedPath
 * @param serverAddress
 * @param port
 * @return
 */
SeedItem*
SakuraRoot::prepareSeed(const std::string &seedPath)
{
    Kitsunemimi::Sakura::SakuraParsing sakuraParsing(m_enableDebug);

    // prepare path for parser
    const std::string parent = Kitsunemimi::Persistence::getParent(seedPath);
    const std::string relPath = Kitsunemimi::Persistence::getRelativePath(seedPath, parent);

    // parse seed-file
    std::string errorMessage = "";
    SakuraItem* seed = sakuraParsing.parseSingleFile(relPath, parent, errorMessage);

    // check parser-result
    if(seed == nullptr
           || seed->getType() != SakuraItem::SEED_ITEM)
    {
        return nullptr;
    }

    return static_cast<SeedItem*>(seed);
}

/**
 * @brief SakuraRoot::shareAllTrees
 */
void
SakuraRoot::shareAllTrees()
{
    std::map<std::string, TreeItem*>::const_iterator it;
    for(it = m_treeHandler->m_garden.trees.begin();
        it != m_treeHandler->m_garden.trees.end();
        it++)
    {
        m_networking->sendTreePlanToAll(it->first, it->second->unparsedConent);
    }
}

}
