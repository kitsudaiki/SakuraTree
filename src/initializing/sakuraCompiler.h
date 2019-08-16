/**
 *  @file    sakuraCompiler.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURACOMPILER_H
#define SAKURACOMPILER_H

#include <common.h>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>

namespace Kitsune
{
namespace Sakura
{
class SakuraConverter;
}
}

namespace SakuraTree
{
class FileCollector;

class SakuraCompiler
{
public:
    SakuraCompiler(Kitsune::Sakura::SakuraConverter* driver);
    ~SakuraCompiler();

    DataObject* compile(const std::string &rootPath,
                        std::string &seedName);

private:
    Kitsune::Sakura::SakuraConverter* m_driver = nullptr;
    FileCollector* m_fileCollector = nullptr;

    void processObject(DataObject* value);
    void processArray(DataArray* value);
};

}

#endif // SAKURACOMPILER_H
