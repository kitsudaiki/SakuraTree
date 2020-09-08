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

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/process_execution.h>

/**
 * @brief constructor
 */
SshScpBlossom::SshScpBlossom()
    : Blossom()
{
    m_requiredKeys.insert("user", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("address", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("target_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("source_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("port", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("ssh_key", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
void
SshScpBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string user = blossomItem.values.getValueAsString("user");
    const std::string address = blossomItem.values.getValueAsString("address");
    const std::string targetPath = blossomItem.values.getValueAsString("target_path");
    const std::string sourcePath = blossomItem.values.getValueAsString("source_path");
    const std::string port = blossomItem.values.getValueAsString("port");
    const std::string sshKey = blossomItem.values.getValueAsString("ssh_key");


    std::string programm = "scp ";

    if(port != "") {
        programm += " -P " + port;
    }

    if(sshKey != "") {
        programm += " -i " + sshKey;
    }

    programm += " ";
    programm += sourcePath;
    programm += " ";
    programm += user;
    programm += "@";
    programm += address;
    programm += ":";
    programm += targetPath;

    LOG_DEBUG("run command: " + programm);

    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
}
