/**
 * @file        ssh_blossoms.cpp
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

#include "ssh_blossoms.h"

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/process_execution.h>

//==================================================================================================
// SshCmdBlossom
//==================================================================================================
SshCmdBlossom::SshCmdBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("user", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("address", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("command", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("port", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("ssh_key", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
bool
SshCmdBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const std::string user = blossomItem.values.getValueAsString("user");
    const std::string address = blossomItem.values.getValueAsString("address");
    const std::string command = blossomItem.values.getValueAsString("command");
    const std::string port = blossomItem.values.getValueAsString("port");
    const std::string sshKey = blossomItem.values.getValueAsString("ssh_key");

    std::string programm = "ssh ";
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
    programm += " -T ";
    programm += "\"";
    programm += command;
    programm += "\"";

    LOG_DEBUG("run command: " + programm);
    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(programm);
    if(processResult.success == false)
    {
        errorMessage = processResult.processOutput;
        return false;
    }

    blossomItem.blossomOutput.insert("output",
                                     new Kitsunemimi::DataValue(processResult.processOutput));

    return true;
}

//==================================================================================================
// SshCmdCreateFileBlossom
//==================================================================================================
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
bool
SshCmdCreateFileBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
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

    if(processResult.success == false)
    {
        errorMessage = processResult.processOutput;
        return false;
    }


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

    if(processResult.success == false)
    {
        errorMessage = processResult.processOutput;
        return false;
    }

    return true;
}

//==================================================================================================
// SshScpBlossom
//==================================================================================================
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
bool
SshScpBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
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

    if(processResult.success == false)
    {
        errorMessage = processResult.processOutput;
        return false;
    }

    return true;
}
