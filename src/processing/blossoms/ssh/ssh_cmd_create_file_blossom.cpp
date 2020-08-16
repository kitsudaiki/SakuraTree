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

#include <libKitsunemimiCommon/common_methods/string_methods.h>

SshCmdCreateFileBlossom::SshCmdCreateFileBlossom()
    : Blossom()
{
    m_requiredKeys.insert("user", new DataValue(true));
    m_requiredKeys.insert("address", new DataValue(true));
    m_requiredKeys.insert("file_content", new DataValue(true));
    m_requiredKeys.insert("file_path", new DataValue(true));
    m_requiredKeys.insert("port", new DataValue(false));
    m_requiredKeys.insert("ssh_key", new DataValue(false));
}

/**
 * initBlossom
 */
void
SshCmdCreateFileBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_user = blossomItem.values.getValueAsString("user");
    m_address = blossomItem.values.getValueAsString("address");
    m_fileContent = blossomItem.values.getValueAsString("file_content");
    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_port = blossomItem.values.getValueAsString("port");
    m_sshKey = blossomItem.values.getValueAsString("ssh_key");

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
    programm += " -T \"sudo rm ";
    programm += m_filePath;
    programm += "\"";

    LOG_DEBUG("run command: " + programm);
    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    ProcessResult processResult = runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;


    programm = "";

    programm += "echo \"";
    programm += m_fileContent;
    programm += "\" | ";

    programm += "ssh ";
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
    // with "cat" instead of "tee" it doesn't work for files in root-context
    programm += " -T \"sudo tee -a ";
    programm += m_filePath;
    programm += "\"";

    LOG_DEBUG("run command: " + programm);
    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    processResult = runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
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
 * closeBlossom
 */
void
SshCmdCreateFileBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
