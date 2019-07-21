/**
 *  @file    parserInit.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURAPARSERINIT_H
#define SAKURAPARSERINIT_H

#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace Kitsune
{
namespace Json
{
class AbstractJson;
class JsonObject;
}
namespace Sakura
{
class LibKitsuneSakuraParser;
}
}

using namespace boost::filesystem;
using Kitsune::Sakura::LibKitsuneSakuraParser;
using Kitsune::Json::JsonObject;
using Kitsune::Json::AbstractJson;

namespace SakuraSeed
{

class FileCollector
{
public:
    FileCollector(LibKitsuneSakuraParser* driver);

    bool initFileCollector(const std::string &rootPath);

    JsonObject* getObject(const std::string &name,
                          const std::string &type="");
    const std::string getSeedName(const uint32_t index);
    const std::string getErrorMessage() const;

private:
    LibKitsuneSakuraParser* m_driver = nullptr;

    void getFilesInDir(const path &directory);
    const std::string readFile(const std::string &filePath);

    std::vector<std::pair<std::string, JsonObject*>> m_fileContents;
    std::string m_errorMessage = "";
};

}

#endif // SAKURAPARSERINIT_H
