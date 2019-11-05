/**
 * @file        scp_blossom.cpp
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

#include "ssh_blossom.h"
#include <processing/process_methods.h>

namespace SakuraTree
{

SshBlossom::SshBlossom()
{

}

/**
 * initTask
 */
void
SshBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.values.contains("user") == false
            || blossomItem.values.contains("address") == false)
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
SshBlossom::preCheck(BlossomItem &blossomItem)
{
    //TODO: check per ssh if file already exist
    blossomItem.success = true;
}

/**
 * runTask
 */
void
SshBlossom::runTask(BlossomItem &blossomItem)
{
    std::string programm = "ssh ";
    if(blossomItem.values.contains("port")) {
        programm += " -p " + blossomItem.values.getStringByKey("port");
    }
    if(blossomItem.values.contains("ssh_key")) {
        programm += " -i " + blossomItem.values.getStringByKey("ssh_key");
    }

    programm += blossomItem.values.getStringByKey("user");
    programm += "@";
    programm += blossomItem.values.getStringByKey("address");
    programm += ":";

    runSyncProcess(blossomItem, programm);
}

/**
 * postCheck
 */
void
SshBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
SshBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
