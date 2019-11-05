/**
 * @file        file_collector.h
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

#ifndef FILE_COLLECTOR_H
#define FILE_COLLECTOR_H

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

namespace Kitsunemimi
{
namespace Common
{
class DataItem;
class DataMap;
}
namespace Sakura
{
class SakuraConverter;
}
}

using namespace boost::filesystem;
using Kitsunemimi::Sakura::SakuraConverter;

namespace SakuraTree
{

class FileCollector
{
public:
    FileCollector(SakuraConverter* driver);

    bool initFileCollector(const std::string &rootPath);

    DataMap* getObject(const std::string &name,
                          const std::string &type="");
    const std::string getSeedName(const uint32_t index);
    const std::string getErrorMessage() const;

private:
    SakuraConverter* m_driver = nullptr;

    void getFilesInDir(const path &directory);
    const std::string readFile(const std::string &filePath);

    std::vector<std::pair<std::string, DataMap*>> m_fileContents;
    std::string m_errorMessage = "";
};

}

#endif // FILE_COLLECTOR_H
