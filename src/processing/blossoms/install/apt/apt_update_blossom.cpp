/**
 * @file        apt_update_blossom.cpp
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

#include "apt_update_blossom.h"
#include <processing/blossoms/install/apt/apt_methods.h>

namespace SakuraTree
{

AptUdateBlossom::AptUdateBlossom()
    : Blossom() {}

/**
 * initBlossom
 */
void
AptUdateBlossom::initBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
AptUdateBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
AptUdateBlossom::runTask(BlossomItem &blossomItem)
{
    const std::string programm = "sudo apt-get update";
    blossomItem.processResult = runSyncProcess(programm);
    blossomItem.success = blossomItem.processResult.success;
    blossomItem.outputMessage = "";
}

/**
 * postCheck
 */
void
AptUdateBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
AptUdateBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
