/**
 * @file        apt_upgrade_blossom.cpp
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

#include "apt_upgrade_blossom.h"
#include <blossoms/install/apt/apt_methods.h>

AptUpgradeBlossom::AptUpgradeBlossom()
    : Blossom() {}

Kitsunemimi::Sakura::Blossom*
AptUpgradeBlossom::createNewInstance()
{
    return new AptUpgradeBlossom();
}

/**
 * initBlossom
 */
void
AptUpgradeBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
AptUpgradeBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
AptUpgradeBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string programm = "sudo apt-get -y upgrade";
    LOG_DEBUG("run command: " + programm);

    ProcessResult processResult = runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
}

/**
 * postCheck
 */
void
AptUpgradeBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
AptUpgradeBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
