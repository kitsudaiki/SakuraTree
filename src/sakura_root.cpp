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
#include <sakura_compiler.h>

#include <items/item_methods.h>
#include <processing/sakura_thread.h>
#include <processing/sakura_tree_callbacks.h>

#include <libKitsunemimiSakuraNetwork/sakura_host_handler.h>
#include <libKitsunemimiSakuraParser/file_collector.h>

#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiJinja2/jinja2_converter.h>

namespace SakuraTree
{

SakuraRoot* SakuraRoot::m_root = nullptr;
Jinja2Converter* SakuraRoot::m_jinja2Converter = nullptr;
std::string SakuraRoot::m_executablePath = "";

/**
 * constructor
 */
SakuraRoot::SakuraRoot(const std::string &executablePath)
{
    m_root = this;
    m_jinja2Converter = new Kitsunemimi::Jinja2::Jinja2Converter;
    m_executablePath = executablePath;

    m_controller = new Kitsunemimi::Sakura::SakuraHostHandler(this,
                                                              &sessionCallback,
                                                              &dataCallback,
                                                              &blossomOutputCallback);
}

/**
 * destructor
 */
SakuraRoot::~SakuraRoot()
{
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
                         std::string seedName)
{
    // m_controller->createServer(1337);

    // parsing
    FileCollector fileCollector;
    SakuraCompiler compiler;
    JsonItem tree = fileCollector.parseFiles(rootPath, seedName, DEBUG);
    SakuraItem* processPlan = compiler.compile(tree);

    assert(processPlan != nullptr);

    // run process
    ValueItemMap dummyObj;
    m_rootThread = new SakuraThread(processPlan, dummyObj, std::vector<std::string>());
    m_rootThread->startThread();
    m_rootThread->waitUntilStarted();
    m_rootThread->waitForFinish();

    // sleep(1000);
    std::cout<<"finish"<<std::endl;

    return true;
}

/**
 * @brief SakuraRoot::startSubtreeProcess
 * @param subtree
 * @return
 */
bool
SakuraRoot::startSubtreeProcess(const std::string &subtree,
                                const std::string &values)
{
    std::cout<<"startSubtreeProcess"<<std::endl;
    // parsing
    JsonItem completePlan;
    completePlan.parse(subtree);

    SakuraCompiler compiler;
    SakuraItem* processPlan = compiler.compile(completePlan);

    assert(processPlan != nullptr);

    // run process
    JsonItem valuesJson;
    valuesJson.parse(values);
    /*m_rootThread = new SakuraThread(processPlan,
                                    *valuesJson.getItemContent()->copy()->toMap(),
                                    std::vector<std::string>());*/
    m_rootThread->startThread();
    m_rootThread->waitUntilStarted();

    std::cout<<"finish"<<std::endl;

    return true;
}

/**
 * @brief SakuraRoot::sendPlan
 * @param address
 * @param plan
 * @return
 */
bool
SakuraRoot::sendPlan(const std::string &address,
                     const std::string &plan,
                     const std::string &values)
{
    return m_controller->sendGrowPlan(address, plan, values);
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
    return m_controller->startTcpSession(address, static_cast<uint16_t>(port));
}

/**
 * @brief SakuraRoot::pringOutpue
 */
void
SakuraRoot::printOutput(const BlossomItem &blossomItem)
{
    m_mutex.lock();

    std::cout<<" "<<std::endl;
    std::string output = convertBlossomOutput(blossomItem);

    // only for prototyping hardcoded
    m_controller->sendBlossomOuput("127.0.0.1", output);
    std::cout<<output<<std::endl;

    m_mutex.unlock();
}

/**
 * @brief SakuraRoot::pringOutput
 * @param output
 */
void
SakuraRoot::printOutput(const std::string &output)
{
    m_mutex.lock();
    std::cout<<output<<std::endl;
    m_mutex.unlock();
}

}
