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
#include <converter/converter.h>

#include <processing/common/item_methods.h>
#include <processing/sakura_thread.h>
#include <processing/sakura_tree_callbacks.h>
#include <processing/thread_pool.h>

// TODO: enable again in 0.3.0
// #include <libKitsunemimiSakuraNetwork/sakura_host_handler.h>
#include <libKitsunemimiSakuraParser/sakura_parsing.h>

#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiJinja2/jinja2_converter.h>

namespace SakuraTree
{

SakuraRoot* SakuraRoot::m_root = nullptr;
std::string SakuraRoot::m_executablePath = "";
TableItem SakuraRoot::m_errorOutput;
Jinja2Converter* SakuraRoot::m_jinja2Converter = nullptr;

/**
 * @brief constructor
 *
 * @param executablePath path of the current executed SakuraTree-binary
 */
SakuraRoot::SakuraRoot(const std::string &executablePath)
{
    // initialzed static variables
    m_root = this;
    m_executablePath = executablePath;
    m_jinja2Converter = new Jinja2Converter();

    // initialize error-output
    m_errorOutput.addColumn("Field");
    m_errorOutput.addColumn("Value");

    // initialize thread-pool
    // TODO: make the number of initialized threads configurable
    m_threadPool = new ThreadPool(8);

    // TODO: enable again in 0.3.0
    // m_controller = new Kitsunemimi::Sakura::SakuraHostHandler(this,
    //                                                           &sessionCallback,
    //                                                           &dataCallback,
    //                                                           &blossomOutputCallback);
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
 * @return true if successful, else false
 */
bool
SakuraRoot::startProcess(const std::string &rootPath,
                         const std::string &seedName,
                         const DataMap &initialValues)
{
    // TODO: enable again in 0.3.0
    // m_controller->createServer(1337);

    // parse all files and convert the into
    SakuraParsing sakuraParsing(DEBUG);
    bool parserResult = sakuraParsing.parseFiles(rootPath);
    if(parserResult == false)
    {
        std::cout<<sakuraParsing.getError().toString()<<std::endl;
        return false;
    }

    // get the initial selected parsed file-content. if seedName is empty string, it
    // return the first file. This is helpful, if only one file was parsed
    JsonItem tree = sakuraParsing.getParsedFileContent(seedName);

    // convert json-representaion of the tree into a sakura-item-tree
    Converter converter;
    SakuraItem* processPlan = converter.convert(tree);
    if(processPlan == nullptr)
    {
        std::cout<<m_errorOutput.toString()<<std::endl;
        return false;
    }

    // run process by adding the tree as subtree-object to the subtree-queue to be processed by
    // one of the worker-threads
    SubtreeQueue::SubtreeObject object;
    object.subtree = processPlan;
    object.items = initialValues;
    object.activeCounter = new SubtreeQueue::ActiveCounter();
    object.activeCounter->shouldCount = 1;
    m_threadPool->m_queue.addSubtreeObject(object);

    // wait until the created subtree was fully processed by the worker-threads
    while(object.activeCounter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    std::cout<<"finish"<<std::endl;

    return true;
}

/**
 * readded in 0.3.0
 */
bool
SakuraRoot::startSubtreeProcess(const std::string &subtree,
                                const std::string &values)
{
    std::cout<<"startSubtreeProcess"<<std::endl;
    // parsing
    JsonItem completePlan;
    completePlan.parse(subtree);

    Converter converter;
    SakuraItem* processPlan = converter.convert(completePlan);
    if(processPlan == nullptr) {
        return false;
    }

    // run process
    JsonItem valuesJson;
    valuesJson.parse(values);
    // TODO: enable again in 0.3.0
    // m_rootThread = new SakuraThread(processPlan,
    //                                 *valuesJson.getItemContent()->copy()->toMap(),
    //                                 std::vector<std::string>());
    // m_rootThread->startThread();
    // m_rootThread->waitUntilStarted();

    std::cout<<"finish"<<std::endl;

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
 * readded in 0.3.0
 */
bool
SakuraRoot::sendPlan(const std::string &address,
                     const std::string &subtree,
                     const std::string &values)
{
    // TODO: enable again in 0.3.0
    // return m_controller->sendGrowPlan(address, subtree, values);
    return false;
}


/**
 * readded in 0.3.0
 */
bool
SakuraRoot::startClientConnection(const std::string &address,
                                  const int port)
{
    // TODO: enable again in 0.3.0
    // return m_controller->startTcpSession(address, static_cast<uint16_t>(port));
    return false;
}

/**
 * readded in 0.3.0
 */
void
SakuraRoot::printOutput(const BlossomItem &blossomItem)
{
    m_mutex.lock();

    std::cout<<" "<<std::endl;
    std::string output = convertBlossomOutput(blossomItem);

    // only for prototyping hardcoded
    // TODO: enable again in 0.3.0
    // m_controller->sendBlossomOuput("127.0.0.1", output);
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

}
