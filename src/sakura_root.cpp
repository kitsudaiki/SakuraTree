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
#include <libKitsunemimiJinja2/jinja2_converter.h>

#include <initializing/sakura_compiler.h>
#include <items/item_methods.h>
#include <processing/sakura_thread.h>

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
    // parsing
    SakuraConverter* sakuraParser = new SakuraConverter(DEBUG);
    SakuraCompiler compiler(sakuraParser);
    SakuraItem* processPlan = compiler.compile(rootPath, seedName);

    assert(processPlan != nullptr);

    // run process
    DataMap* dummyObj = new DataMap();
    m_rootThread = new SakuraThread(processPlan, dummyObj, std::vector<std::string>());
    m_rootThread->startThread();
    m_rootThread->waitUntilStarted();
    m_rootThread->waitForFinish();

    std::cout<<"finish"<<std::endl;

    return true;
}

/**
 * @brief SakuraRoot::addMessage
 */
void
SakuraRoot::addMessage(const BlossomItem &blossomItem)
{
    m_mutex.lock();
    std::cout<<" "<<std::endl;
    printOutput(blossomItem);
    m_mutex.unlock();
}

}
