/**
 *  @file    sakura_root.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
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
