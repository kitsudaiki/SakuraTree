/**
 * @file        ssh_cmd_create_file_blossom.cpp
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

#include "ssh_cmd_create_file_blossom.h"
#include <processing/process_execution.h>

namespace SakuraTree
{

SshCmdCreateFileBlossom::SshCmdCreateFileBlossom()
{

}

/**
 * initTask
 */
void
SshCmdCreateFileBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues.contains("user") == false
            || blossomItem.inputValues.contains("address") == false
            || blossomItem.inputValues.contains("file_path") == false
            || blossomItem.inputValues.contains("file_content") == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "missing connection informations";
    }

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
SshCmdCreateFileBlossom::preCheck(BlossomItem &blossomItem)
{
    //TODO: check per ssh if file already exist
    blossomItem.success = true;
}

/**
 * runTask
 */
void
SshCmdCreateFileBlossom::runTask(BlossomItem &blossomItem)
{
    std::string programm = "";

    programm += "ssh ";
    if(blossomItem.inputValues.contains("port")) {
        programm += " -p " + blossomItem.inputValues.get("port")->getString();
    }
    if(blossomItem.inputValues.contains("ssh_key")) {
        programm += " -i " + blossomItem.inputValues.get("ssh_key")->getString();
    }

    programm += " ";
    programm += blossomItem.inputValues.get("user")->getString();
    programm += "@";
    programm += blossomItem.inputValues.get("address")->getString();
    programm += " -T \"sudo rm ";
    programm += blossomItem.inputValues.get("file_path")->getString();
    programm += "\"";

    runSyncProcess(blossomItem, programm);


    programm = "";

    programm += "echo \"";
    programm += blossomItem.inputValues.get("file_content")->getString();
    programm += "\" | ";

    programm += "ssh ";
    if(blossomItem.inputValues.contains("port")) {
        programm += " -p " + blossomItem.inputValues.get("port")->getString();
    }
    if(blossomItem.inputValues.contains("ssh_key")) {
        programm += " -i " + blossomItem.inputValues.get("ssh_key")->getString();
    }

    programm += " ";
    programm += blossomItem.inputValues.get("user")->getString();
    programm += "@";
    programm += blossomItem.inputValues.get("address")->getString();
    // with "cat" instead of "tee" it doesn't work for files in root-context
    programm += " -T \"sudo tee -a ";
    programm += blossomItem.inputValues.get("file_path")->getString();
    programm += "\"";

    runSyncProcess(blossomItem, programm);
}

/**
 * postCheck
 */
void
SshCmdCreateFileBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
SshCmdCreateFileBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
