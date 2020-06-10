/**
 * @file        sakura_root.h
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

#ifndef SAKURA_ROOT_H
#define SAKURA_ROOT_H

#include <common.h>
#include <unistd.h>
#include <libKitsunemimiCommon/common_items/table_item.h>

namespace Kitsunemimi
{
namespace Jinja2 {
class Jinja2Converter;
}
namespace Sakura {
class SakuraNetwork;
}
namespace Project {
class Session;
}
}

using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{
class SakuraThread;
class ThreadPool;
class TreeHandler;

class SakuraRoot
{

public:
    SakuraRoot(const std::string &executablePath,
               const bool enableDebug = false);
    ~SakuraRoot();

    // start processing
    bool startProcess(const std::string &configFilePath);
    bool startProcess(const std::string &inputPath,
                      const std::string &seedPath,
                      const DataMap &initialValues,
                      const std::string &initialTreeId = "",
                      const std::string &serverAddress = "",
                      const uint16_t serverPort = 1337);
    bool startSubtreeProcess(const std::string &relativePath,
                             const std::string &values,
                             Kitsunemimi::Project::Session* session,
                             const uint64_t blockerId);

    // error-output
    void createError(const BlossomItem &blossomItem,
                     const std::string &errorLocation,
                     const std::string &errorMessage,
                     const std::string &possibleSolution = "");
    void createError(const std::string &errorLocation,
                     const std::string &errorMessage,
                     const std::string &possibleSolution = "",
                     const std::string &blossomType = "",
                     const std::string &blossomGroupType = "",
                     const std::string &blossomName = "",
                     const std::string &blossomFilePath = "");

    // output
    void printOutput(const BlossomItem &blossomItem);
    void printOutput(const std::string &output);

    // static values
    static SakuraTree::SakuraRoot* m_root;
    static std::string m_executablePath;
    static Jinja2Converter* m_jinja2Converter;
    static Kitsunemimi::TableItem m_errorOutput;
    static SakuraTree::TreeHandler* m_treeHandler;
    static Kitsunemimi::Sakura::SakuraNetwork* m_networking;

    bool m_enableDebug = false;

private:
    SakuraThread* m_rootThread = nullptr;
    ThreadPool* m_threadPool = nullptr;

    std::mutex m_mutex;

    bool runProcess(SakuraItem* item,
                    const DataMap &initialValues);
    bool processSeed(const std::string &seedPath,
                     const std::string &serverAddress,
                     const uint16_t serverPort,
                     std::string &errorMessage);
    SeedItem* prepareSeed(const std::string &seedPath,
                          std::string &errorMessage);
};

}

#endif // SAKURA_ROOT_H
