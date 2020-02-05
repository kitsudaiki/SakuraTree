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
}

using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{
class SakuraThread;
class ThreadPool;
class TreeHandler;
class SakuraItem;
struct BlossomItem;

class SakuraRoot
{

public:
    SakuraRoot(const std::string &executablePath);
    ~SakuraRoot();

    // start processing
    bool startProcess(const std::string &initialTreePath,
                      const std::string &seedPath,
                      const DataMap &initialValues,
                      const std::string &serverAddress="127.0.0.1",
                      const uint16_t serverPort=1337);
    bool startSubtreeProcess(const std::string &subtree,
                             const std::string &values);

    // error-output
    void createError(const BlossomItem &blossomItem,
                     const std::string &errorLocation,
                     const std::string &errorMessage,
                     const std::string &possibleSolution="");
    void createError(const std::string &errorLocation,
                     const std::string &errorMessage,
                     const std::string &possibleSolution="",
                     const std::string &blossomType="",
                     const std::string &blossomGroupType="",
                     const std::string &blossomName="",
                     const std::string &blossomFilePath="");

    // network-interaction
    bool sendTreefile(const std::string &address,
                      const std::string &subtree,
                      const std::string &values);
    bool startClientConnection(const std::string &address,
                               const int port);

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

private:
    SakuraThread* m_rootThread = nullptr;
    ThreadPool* m_threadPool = nullptr;

    std::mutex m_mutex;

    bool runProcess(SakuraItem* item,
                    const DataMap &initialValues);
    SakuraItem* prepareSeed(const std::string &seedPath,
                            const std::string &executablePath,
                            const std::string &serverAddress,
                            const uint16_t port);
};

}

#endif // SAKURA_ROOT_H
