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
#include <config.h>

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

#include <sakura_provisioning_subtree.h>

namespace SakuraTree
{

SakuraRoot* SakuraRoot::m_root = nullptr;
std::string SakuraRoot::m_executablePath = "";
std::string SakuraRoot::m_serverAddress = "127.0.0.1";
uint16_t SakuraRoot::m_serverPort = 1337;
TableItem SakuraRoot::m_errorOutput;
Jinja2Converter* SakuraRoot::m_jinja2Converter = nullptr;
Kitsunemimi::Sakura::SakuraGarden* SakuraRoot::m_currentGarden = nullptr;
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
    m_currentGarden = new Kitsunemimi::Sakura::SakuraGarden();

    // initialize error-output
    m_errorOutput.addColumn("Field");
    m_errorOutput.addColumn("Value");

    // initialize thread-pool
    // TODO: make the number of initialized threads configurable
    m_threadPool = new ThreadPool(8);

    m_networking = new Kitsunemimi::Sakura::SakuraNetwork(this,
                                                          &sessionCallback,
                                                          &objectTransferCallback,
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
    bool success = Kitsunemimi::Config::initConfig(configFilePath);
    if(success == false) {
        return false;
    }

    // load config definition
    SakuraTree::registerConfigs();

    const bool debug = GET_BOOL_CONFIG("DEFAULT", "debug", success);
    Kitsunemimi::Persistence::initFileLogger("/tmp/", "SakuraTree_log", debug);
    Kitsunemimi::Persistence::setDebugFlag(debug);

    const std::string address = GET_STRING_CONFIG("server", "server_address", success);
    // TODO: log-error
    assert(success);
    const uint16_t port = static_cast<uint16_t>(GET_INT_CONFIG("server", "server_port", success));
    // TODO: log-error
    assert(success);
    m_networking->createClientConnection(address, port);

    // TODO: better solution
    while(true) {
        sleep(1);
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
                         const DataMap &initialValues,
                         const std::string &initialTreeId,
                         const std::string &serverAddress,
                         const uint16_t serverPort)
{
    std::string errorMessage = "";

    m_serverAddress = serverAddress;
    m_serverPort = serverPort;

    // load predefined trees
    if(loadPredefinedSubtrees(errorMessage) == false)
    {
        LOG_ERROR("Failed to load predefined trees");
        LOG_ERROR("    " + errorMessage);
        return false;
    }

    // process real task
    if(m_currentGarden->addTree(inputPath, errorMessage) == false)
    {
        LOG_ERROR("failed to add trees\n    " + errorMessage);
        return false;
    }

    SakuraItem* tree = nullptr;

    // get initial tree-file
    if(Kitsunemimi::Persistence::isFile(inputPath))
    {
        tree = m_currentGarden->getTree(inputPath);
    }
    else
    {
        if(initialTreeId != "") {
            tree = m_currentGarden->getTreeById(initialTreeId);
        } else {
            tree = m_currentGarden->getTree(inputPath);
        }

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

    // process tree-file with initial values
    if(runProcess(tree, initialValues) == false)
    {
        LOG_ERROR(m_errorOutput.toString());
        return false;
    }

    LOG_INFO("finish");

    // close connection to all hosts
    m_networking->closeAllSessions();

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
    SakuraItem* processPlan = m_currentGarden->getTree(relativePath);
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
 * @brief SakuraRoot::printOutput
 * @param blossomItem
 */
void
SakuraRoot::printOutput(const BlossomGroupItem &blossomGroupItem)
{
    std::string output = "";

    // print call-hierarchy
    for(uint32_t i = 0; i < blossomGroupItem.nameHirarchie.size(); i++)
    {
        for(uint32_t j = 0; j < i; j++)
        {
            output += "   ";
        }
        output += blossomGroupItem.nameHirarchie.at(i) + "\n";
    }

    printOutput(output);
}

/**
 * @brief SakuraRoot::printOutput
 * @param blossomItem
 */
void
SakuraRoot::printOutput(const BlossomItem &blossomItem)
{
    const std::string output = convertBlossomOutput(blossomItem);

    // only for prototyping hardcoded
    //m_networking->sendBlossomOuput("127.0.0.1", "", output);
    printOutput(output);
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

    // get width of the termial to draw the separator-line
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    const uint32_t terminalWidth = size.ws_col;

    // draw separator line
    std::string line(terminalWidth, '=');

    std::cout<<line<<"\n\n"<<output<<"\n"<<std::endl;

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
 * @brief load predefined subtrees
 */
bool
SakuraRoot::loadPredefinedSubtrees(std::string &errorMessage)
{
    // get predifile provisioning subtree
    std::string provisioningSubtree(reinterpret_cast<char*>(sakura_provisioning_subtree_tree),
                                    sakura_provisioning_subtree_tree_len);
    Kitsunemimi::replaceSubstring(provisioningSubtree, "\\n", "\n");

    bool ret = m_currentGarden->addResource(provisioningSubtree, errorMessage);
    if(ret == false) {
        return false;
    }

    return true;
}

}
