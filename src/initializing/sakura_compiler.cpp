/**
 *  @file    sakura_compiler.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "sakura_compiler.h"
#include <sakura_converter.h>
#include <initializing/file_collector.h>

namespace SakuraTree
{

/**
 * @brief SakuraCompiler::SakuraCompiler
 * @param driver
 */
SakuraCompiler::SakuraCompiler(Kitsune::Sakura::SakuraConverter *driver)
{
    m_driver = driver;
    m_fileCollector = new FileCollector(m_driver);
}

SakuraCompiler::~SakuraCompiler()
{
    delete m_fileCollector;
}

/**
 * @brief SakuraCompiler::compile
 * @param type
 * @param name
 * @return
 */
DataMap*
SakuraCompiler::compile(const std::string &rootPath,
                        std::string &seedName)
{
    if(m_fileCollector->initFileCollector(rootPath) == false)
    {
        //TODO: replace with better solution
        std::cout<<"ERROR: "<<m_fileCollector->getErrorMessage()<<std::endl;
        exit(1);
    }

    if(seedName == "") {
        seedName = m_fileCollector->getSeedName(0);
    }

    DataMap* result = m_fileCollector->getObject(seedName);
    assert(result != nullptr);
    processObject(result);

    return result;
}

/**
 * @brief SakuraCompiler::processObject
 * @param value
 * @return
 */
void
SakuraCompiler::processObject(DataMap* value)
{
    // precheck
    if(value == nullptr) {
        return;
    }

    // end of tree
    if(value->get("type")->toString() == "blossom") {
        return;
    }

    // continue building

    DataMap* branch = value;
    if(value->get("type")->toString() != "parallel"
            && value->get("type")->toString() != "sequentiell")
    {
        branch = m_fileCollector->getObject(value->get("name")->toString());
        value->insert("parts", branch->get("parts"));
        value->insert("items", branch->get("items"));
    }
    processArray(value->get("parts")->toArray());
}

/**
 * @brief SakuraCompiler::processArray
 * @param value
 * @return
 */
void
SakuraCompiler::processArray(DataArray* value)
{
    for(uint32_t i = 0; i < value->size(); i++)
    {
        processObject(value->get(i)->toMap());
    }
}

}
