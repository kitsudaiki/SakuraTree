/**
 *  @file    sakuraRoot.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "sakuraRoot.h"
#include <jinja2Converter.hpp>

#include <initializing/sakuraCompiler.h>
#include <commonMethods.h>
#include <processing/sakuraThread.h>

namespace SakuraTree
{

SakuraRoot* SakuraRoot::m_root = nullptr;
Jinja2Converter* SakuraRoot::m_jinja2Converter = nullptr;

/**
 * constructor
 */
SakuraRoot::SakuraRoot()
{
    m_root = this;
    m_jinja2Converter = new Kitsune::Jinja2::Jinja2Converter;
}

/**
 * destructor
 */
SakuraRoot::~SakuraRoot()
{
    if(m_rootThread != nullptr)
    {
        m_rootThread->stop();
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
    DataObject* processPlan = compiler.compile(rootPath, seedName);

    // debug-output
    if(DEBUG)
    {
        std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        std::string output = "";
        processPlan->print(&output, true);
        std::cout<<output<<std::endl;
        std::cout<<"-----------------------------------------------------"<<std::endl;
    }

    // run process
    DataObject* dummyObj = new DataObject();
    m_rootThread = new SakuraThread(processPlan, dummyObj, std::vector<std::string>());
    m_rootThread->start();
    m_rootThread->waitUntilStarted();
    m_rootThread->waitForFinish();

    std::cout<<"finish"<<std::endl;

    return true;
}

/**
 * @brief SakuraRoot::addMessage
 */
void
SakuraRoot::addMessage(BlossomData* blossomData)
{
    m_mutex.lock();
    std::cout<<" "<<std::endl;
    printOutput(blossomData);
    m_mutex.unlock();
}

}
