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

#include <common.h>
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
namespace Common
{
class DataItem;
class DataObject;
}
namespace Sakura
{
class SakuraConverter;
}
}

using namespace boost::filesystem;
using Kitsune::Sakura::SakuraConverter;

namespace SakuraTree
{

class FileCollector
{
public:
    FileCollector(SakuraConverter* driver);

    bool initFileCollector(const std::string &rootPath);

    DataObject* getObject(const std::string &name,
                          const std::string &type="");
    const std::string getSeedName(const uint32_t index);
    const std::string getErrorMessage() const;

private:
    SakuraConverter* m_driver = nullptr;

    void getFilesInDir(const path &directory);
    const std::string readFile(const std::string &filePath);

    std::vector<std::pair<std::string, DataObject*>> m_fileContents;
    std::string m_errorMessage = "";
};

}

#endif // SAKURAPARSERINIT_H
