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
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("user", new DataValue(true));
    m_requiredKeys.insert("address", new DataValue(true));
    m_requiredKeys.insert("command", new DataValue(true));
    m_requiredKeys.insert("port", new DataValue(false));
    m_requiredKeys.insert("ssh_key", new DataValue(false));
}

/**
 * initBlossom
 */
void
SshCmdBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_user = blossomItem.values.getValueAsString("user");
    m_address = blossomItem.values.getValueAsString("address");
    m_command = blossomItem.values.getValueAsString("command");
    m_port = blossomItem.values.getValueAsString("port");
    m_sshKey = blossomItem.values.getValueAsString("ssh_key");

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
    if(m_port != "") {
        programm += " -p " + m_port;
    }
    if(m_sshKey != "") {
        programm += " -i " + m_sshKey;
    }

    programm += " ";
    programm += m_user;
    programm += "@";
    programm += m_address;
    programm += " -T ";
    programm += "\"";
    programm += m_command;
    programm += "\"";

    LOG_DEBUG("run command: " + programm);
    ProcessResult processResult = runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;

    blossomItem.blossomOutput = new DataValue(processResult.processOutput);
    blossomItem.success = true;
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
 * closeBlossom
 */
void
SshCmdBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
