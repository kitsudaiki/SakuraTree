/**
 *  @file    sakura_compiler.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURA_COMPILER_H
#define SAKURA_COMPILER_H

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

    DataMap* compile(const std::string &rootPath,
                        std::string &seedName);

private:
    Kitsune::Sakura::SakuraConverter* m_driver = nullptr;
    FileCollector* m_fileCollector = nullptr;

    void processObject(DataMap* value);
    void processArray(DataArray* value);
};

}

#endif // SAKURA_COMPILER_H
