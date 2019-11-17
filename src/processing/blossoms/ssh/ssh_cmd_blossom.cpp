/**
 * @file        ssh_cmd_blossom.cpp
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

#include "ssh_cmd_blossom.h"
#include <processing/process_methods.h>

namespace SakuraTree
{

SshCmdBlossom::SshCmdBlossom()
{

}

/**
 * initTask
 */
void
SshCmdBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.values->contains("user") == false
            || blossomItem.values->contains("address") == false
            || blossomItem.values->contains("command") == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "missing connection informations";
    }

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
SshCmdBlossom::preCheck(BlossomItem &blossomItem)
{
    //TODO: check per ssh if file already exist
    blossomItem.success = true;
}

/**
 * runTask
 */
void
SshCmdBlossom::runTask(BlossomItem &blossomItem)
{
    std::string programm = "ssh ";
    if(blossomItem.values->contains("port")) {
        programm += " -p " + blossomItem.values->get("port")->getString();
    }
    if(blossomItem.values->contains("ssh_key")) {
        programm += " -i " + blossomItem.values->get("ssh_key")->getString();
    }

    programm += " ";
    programm += blossomItem.values->get("user")->getString();
    programm += "@";
    programm += blossomItem.values->get("address")->getString();
    programm += " -T ";
    programm += "\"";
    programm += blossomItem.values->get("command")->getString();
    programm += "\"";

    runSyncProcess(blossomItem, programm);
    sleep(1);
}

/**
 * postCheck
 */
void
SshCmdBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
SshCmdBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
