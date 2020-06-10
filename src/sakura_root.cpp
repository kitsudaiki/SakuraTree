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
                         const std::string &seedPath,
                         const DataMap &initialValues,
                         const std::string &initialTreeId,
                         const std::string &serverAddress,
                         const uint16_t serverPort)
{
    std::string errorMessage = "";

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
        if(m_networking->createServer(serverPort) == false)
        {
            LOG_ERROR("failed to create server on port " + std::to_string(1337));
            return false;
        }

        // parse and process seed-file
        if(processSeed(seedPath,
                       serverAddress,
                       serverPort,
                       errorMessage) == false)
        {
            LOG_ERROR("failed process seed\n    " + errorMessage);
            return false;
        }
    }

    // process real task
    if(m_treeHandler->addTree(inputPath, errorMessage) == false)
    {
        LOG_ERROR("failed to add trees\n    " + errorMessage);
        return false;
    }

    SakuraItem* tree = nullptr;

    if(Kitsunemimi::Persistence::isFile(inputPath))
    {
        tree = m_treeHandler->getTree(inputPath);
    }
    else
    {
        if(initialTreeId != "") {
            tree = m_treeHandler->getTreeById(initialTreeId);
        } else {
            tree = m_treeHandler->getTree(inputPath);
        }

    }

    m_networking->sendDataToAll(m_treeHandler->m_garden);

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
    SakuraItem* processPlan = m_treeHandler->getTree(relativePath);
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
SakuraRoot::processSeed(const std::string &seedPath,
                        const std::string &serverAddress,
                        const uint16_t serverPort,
                        std::string &errorMessage)
{
    SeedItem* seedItem = prepareSeed(seedPath, errorMessage);
    if(seedItem == nullptr)
    {
        LOG_ERROR("failed to load seed-file " + seedPath);
        return false;
    }

    TreeItem* provisioningTree = m_treeHandler->getTreeById("sakura_provisioning");
    assert(provisioningTree != nullptr);

    DataMap values;
    values.insert("executable_path", new DataValue(m_executablePath), true);
    values.insert("server_port", new DataValue(serverPort), true);
    values.insert("server_ip_address", new DataValue(serverAddress), true);

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

        // set host specific values
        values.insert("target_path", part->values.get("target_path"), true);
        values.insert("client_ip_address", part->values.get("ip_address"), true);
        values.insert("ssh_user", part->values.get("ssh_user"), true);
        values.insert("ssh_port", part->values.get("ssh_port"), true);
        values.insert("ssh_key_path", part->values.get("ssh_key_path"), true);

        // run provisioning for the host
        const bool ret = runProcess(provisioningTree, values);
        if(ret == false) {
            return false;
        }
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

    return true;
}

/**
 * @brief prepare seed for multi-node-access by parsing the seed-file
 *
 * @param seedPath path to the seed-file
 * @param errorMessage reference to error-message for output
 *
 * @return pointer to parsed seed content
 */
SeedItem*
SakuraRoot::prepareSeed(const std::string &seedPath,
                        std::string &errorMessage)
{
    Kitsunemimi::Sakura::SakuraParsing sakuraParsing(m_enableDebug);

    // prepare path for parser
    const std::string parent = Kitsunemimi::Persistence::getParent(seedPath);
    const std::string relPath = Kitsunemimi::Persistence::getRelativePath(seedPath, parent);

    // parse seed-file
    SakuraItem* seed = sakuraParsing.parseSingleFile(relPath, parent, errorMessage)->copy();

    // check parser-result
    if(seed == nullptr) {
        return nullptr;
    }

    // check if parsed file was a seed-file
    if(seed->getType() != SakuraItem::SEED_ITEM)
    {
        LOG_ERROR("parsed file " + seedPath + " is not a seed-file");
        return nullptr;
    }

    return static_cast<SeedItem*>(seed);
}

}
