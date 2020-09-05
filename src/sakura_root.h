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
#include <libKitsunemimiCommon/common_items/table_item.h>

namespace Kitsunemimi
{
namespace Sakura {
class SakuraNetwork;
class SakuraLangInterface;
}
namespace Sakura {
class Session;
}
}

class SakuraRoot
{

public:
    SakuraRoot(const std::string &executablePath);
    ~SakuraRoot();

    // start processing
    bool startProcess(const std::string &inputPath,
                      const DataMap &initialValues,
                      const bool enableDebug,
                      const bool dryRun = false);

    // static values
    static SakuraRoot* m_root;
    static std::string m_executablePath;
    static Kitsunemimi::Sakura::SakuraLangInterface* m_interface;

private:
    void initBlossoms();
};

#endif // SAKURA_ROOT_H
