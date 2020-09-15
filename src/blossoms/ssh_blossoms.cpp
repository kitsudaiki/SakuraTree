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
    validationMap.emplace("user", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("address", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("command", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("port", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
    validationMap.emplace("ssh_key", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
    validationMap.emplace("output", BlossomValidDef(IO_ValueType::OUTPUT_TYPE, false));
}

/**
 * runTask
 */
bool
SshCmdBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string user = blossomLeaf.input.getStringByKey("user");
    const std::string address = blossomLeaf.input.getStringByKey("address");
    const std::string command = blossomLeaf.input.getStringByKey("command");
    const std::string port = blossomLeaf.input.getStringByKey("port");
    const std::string sshKey = blossomLeaf.input.getStringByKey("ssh_key");

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

    blossomLeaf.output.insert("output", new Kitsunemimi::DataValue(processResult.processOutput));

    return true;
}

//==================================================================================================
// SshCmdCreateFileBlossom
//==================================================================================================
SshCmdCreateFileBlossom::SshCmdCreateFileBlossom()
    : Blossom()
{
    validationMap.emplace("user", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("address", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("file_content", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("file_path", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("port", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
    validationMap.emplace("ssh_key", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
}

/**
 * runTask
 */
bool
SshCmdCreateFileBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string user = blossomLeaf.input.getStringByKey("user");
    const std::string address = blossomLeaf.input.getStringByKey("address");
    const std::string fileContent = blossomLeaf.input.getStringByKey("file_content");
    const std::string filePath = blossomLeaf.input.getStringByKey("file_path");
    const std::string port = blossomLeaf.input.getStringByKey("port");
    const std::string sshKey = blossomLeaf.input.getStringByKey("ssh_key");


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
    validationMap.emplace("user", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("address", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("target_path", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("source_path", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("port", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
    validationMap.emplace("ssh_key", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
}

/**
 * runTask
 */
bool
SshScpBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string user = blossomLeaf.input.getStringByKey("user");
    const std::string address = blossomLeaf.input.getStringByKey("address");
    const std::string targetPath = blossomLeaf.input.getStringByKey("target_path");
    const std::string sourcePath = blossomLeaf.input.getStringByKey("source_path");
    const std::string port = blossomLeaf.input.getStringByKey("port");
    const std::string sshKey = blossomLeaf.input.getStringByKey("ssh_key");


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
