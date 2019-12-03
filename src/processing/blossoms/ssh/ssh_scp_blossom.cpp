/**
 * @file        ssh_scp_blossom.cpp
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

#include "ssh_scp_blossom.h"
#include <processing/process_execution.h>

namespace SakuraTree
{

SshScpBlossom::SshScpBlossom()
{

}

/**
 * initTask
 */
void
SshScpBlossom::initTask(BlossomItem &blossomItem)
{
    const std::vector<std::string> requiredKeys = {"user", "address", "target_path", "source_path"};

    checkForRequiredKeys(blossomItem, requiredKeys);
    if(blossomItem.success == false) {
        return;
    }

    m_user = blossomItem.values.getValueAsString("user");
    m_address = blossomItem.values.getValueAsString("address");
    m_targetPath = blossomItem.values.getValueAsString("target_path");
    m_sourcePath = blossomItem.values.getValueAsString("source_path");
    m_port = blossomItem.values.getValueAsString("port");
    m_sshKey = blossomItem.values.getValueAsString("ssh_key");

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
SshScpBlossom::preCheck(BlossomItem &blossomItem)
{
    //TODO: check per ssh if file already exist
    blossomItem.success = true;
}

/**
 * runTask
 */
void
SshScpBlossom::runTask(BlossomItem &blossomItem)
{
    std::string programm = "scp ";
    if(m_port != "") {
        programm += " -P " + m_port;
    }
    if(m_sshKey != "") {
        programm += " -i " + m_sshKey;
    }

    programm += " ";
    programm += m_sourcePath;
    programm += " ";
    programm += m_user;
    programm += "@";
    programm += m_address;
    programm += ":";
    programm += m_targetPath;

    runSyncProcess(blossomItem, programm);
}

/**
 * postCheck
 */
void
SshScpBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
SshScpBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
