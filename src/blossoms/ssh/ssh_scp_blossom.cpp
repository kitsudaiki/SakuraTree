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

Kitsunemimi::Sakura::Blossom*
SshScpBlossom::createNewInstance()
{
    return new SshScpBlossom();
}

/**
 * initBlossom
 */
void
SshScpBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
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
SshScpBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    //TODO: check per ssh if file already exist
    blossomItem.success = true;
}

/**
 * runTask
 */
void
SshScpBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
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

    LOG_DEBUG("run command: " + programm);
    Kitsunemimi::replaceSubstring(programm, "\"", "\\\"");
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
}

/**
 * postCheck
 */
void
SshScpBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
SshScpBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
