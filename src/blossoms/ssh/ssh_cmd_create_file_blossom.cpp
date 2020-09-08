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
#include <libKitsunemimiCommon/process_execution.h>

/**
 * @brief constructor
 */
SshCmdCreateFileBlossom::SshCmdCreateFileBlossom()
    : Blossom()
{
    m_requiredKeys.insert("user", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("address", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("file_content", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("port", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("ssh_key", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
void
SshCmdCreateFileBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string user = blossomItem.values.getValueAsString("user");
    const std::string address = blossomItem.values.getValueAsString("address");
    const std::string fileContent = blossomItem.values.getValueAsString("file_content");
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string port = blossomItem.values.getValueAsString("port");
    const std::string sshKey = blossomItem.values.getValueAsString("ssh_key");


    std::string programm = "";

    programm += "ssh ";
    if(port != "") {
        programm += " -p " + port;
    }
    if(sshKey != "") {
        programm += " -i " + sshKey;
    }

    programm += " ";
    programm += user;
    programm += "@";
    programm += address;
    programm += " -T \"sudo rm ";
    programm += filePath;
    programm += "\"";

    LOG_DEBUG("run command: " + programm);
    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;


    programm = "";

    programm += "echo \"";
    programm += fileContent;
    programm += "\" | ";

    programm += "ssh ";
    if(port != "") {
        programm += " -p " + port;
    }
    if(sshKey != "") {
        programm += " -i " + sshKey;
    }

    programm += " ";
    programm += user;
    programm += "@";
    programm += address;
    // with "cat" instead of "tee" it doesn't work for files in root-context
    programm += " -T \"sudo tee -a ";
    programm += filePath;
    programm += "\"";

    LOG_DEBUG("run command: " + programm);
    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    processResult = Kitsunemimi::runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
}
