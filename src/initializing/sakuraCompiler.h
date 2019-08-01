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

#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <jsonItems.hpp>

using namespace Kitsune::Json;

namespace Kitsune
{
namespace Sakura
{
class LibKitsuneSakuraParser;
}
}

namespace SakuraSeed
{
class FileCollector;

class SakuraCompiler
{
public:
    SakuraCompiler(Kitsune::Sakura::LibKitsuneSakuraParser* driver);
    ~SakuraCompiler();

    JsonObject* compile(const std::string &rootPath,
                        std::string &seedName);

private:
    Kitsune::Sakura::LibKitsuneSakuraParser* m_driver = nullptr;
    FileCollector* m_fileCollector = nullptr;

    void processObject(JsonObject* value);
    void processArray(JsonArray* value);
};

}

#endif // SAKURACOMPILER_H
