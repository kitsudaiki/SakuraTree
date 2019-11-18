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
    if(blossomItem.inputValues->contains("user") == false
            || blossomItem.inputValues->contains("address") == false
            || blossomItem.inputValues->contains("command") == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "missing connection informations";
        return;
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
    if(blossomItem.inputValues->contains("port")) {
        programm += " -p " + blossomItem.inputValues->get("port")->getString();
    }
    if(blossomItem.inputValues->contains("ssh_key")) {
        programm += " -i " + blossomItem.inputValues->get("ssh_key")->getString();
    }

    programm += " ";
    programm += blossomItem.inputValues->get("user")->getString();
    programm += "@";
    programm += blossomItem.inputValues->get("address")->getString();
    programm += " -T ";
    programm += "\"";
    programm += blossomItem.inputValues->get("command")->getString();
    programm += "\"";

    runSyncProcess(blossomItem, programm);
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
