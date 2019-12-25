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
#include <libKitsunemimiSakuraParser/sakura_parsing.h>

namespace Kitsunemimi
{
namespace Jinja2 {
class Jinja2Converter;
}
namespace Sakura {
class SakuraHostHandler;
}
}

using Kitsunemimi::Sakura::SakuraParsing;
using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{
class SakuraThread;
class ThreadPool;
struct BlossomItem;

class SakuraRoot
{

public:
    SakuraRoot(const std::string &executablePath);
    ~SakuraRoot();

    // start processing
    bool startProcess(const std::string &rootPath,
                      const std::string &seedName,
                      const DataMap &initialValues);
    bool startSubtreeProcess(const std::string &subtree,
                             const std::string &values);

    // network-interaction
    bool sendPlan(const std::string &address,
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
    static TableItem m_errorOutput;

private:
    Kitsunemimi::Sakura::SakuraHostHandler* m_controller = nullptr;
    SakuraThread* m_rootThread = nullptr;
    ThreadPool* m_threadPool = nullptr;

    std::mutex m_mutex;
};

}

#endif // SAKURA_ROOT_H
