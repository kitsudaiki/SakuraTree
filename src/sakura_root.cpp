/**
 * @file        sakura_root.cpp
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

#include "sakura_root.h"

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>

#include <libKitsunemimiConfig/config_handler.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

#include <blossoms/apt_blossoms.h>
#include <blossoms/ini_blossoms.h>
#include <blossoms/path_blossoms.h>
#include <blossoms/special_blossoms.h>
#include <blossoms/ssh_blossoms.h>
#include <blossoms/template_blossoms.h>
#include <blossoms/text_blossoms.h>

SakuraRoot* SakuraRoot::m_root = nullptr;
std::string SakuraRoot::m_executablePath = "";
Kitsunemimi::Sakura::SakuraLangInterface* SakuraRoot::m_interface = nullptr;

/**
 * @brief constructor
 *
 * @param executablePath path of the current executed SakuraTree-binary
 */
SakuraRoot::SakuraRoot(const std::string &executablePath)
{
    // initialzed static variables
    m_root = this;
    m_executablePath = executablePath;
    m_interface = new Kitsunemimi::Sakura::SakuraLangInterface();
}

/**
 * destructor
 */
SakuraRoot::~SakuraRoot()
{
}

/**
 * initialize and start rollout-process
 *
 * @param inputPath initial path to parse
 * @param initialValues key-value-pairs to override the values of the initial file
 * @param dryRun true to start a run with parsing and validating, but without execution
 *
 * @return true if successful, else false
 */
bool
SakuraRoot::startProcess(const std::string &inputPath,
                         const DataMap &initialValues,
                         const bool dryRun)
{
    initBlossoms();

    LOG_INFO(ASCII_LOGO, PINK_COLOR);

    // set default-file in case that a directory instead of a file was selected
    std::string treeFile = inputPath;
    if(bfs::is_directory(treeFile)) {
        treeFile = treeFile + "/root.sakura";
    }

    // process
    std::string errorMessage = "";
    const bool result = m_interface->processFiles(treeFile,
                                                  initialValues,
                                                  dryRun,
                                                  errorMessage);

    if(result) {
        LOG_INFO("finish", GREEN_COLOR);
    } else {
        LOG_ERROR(errorMessage);
    }

    return result;
}

/**
 * @brief register all blossoms
 */
void
SakuraRoot::initBlossoms()
{
    assert(m_interface->addBlossom("apt", "absent", new AptAbsentBlossom()));
    assert(m_interface->addBlossom("apt", "latest", new AptLatestBlossom()));
    assert(m_interface->addBlossom("apt", "present", new AptPresentBlossom()));
    assert(m_interface->addBlossom("apt", "update", new AptUdateBlossom()));
    assert(m_interface->addBlossom("apt", "upgrade", new AptUpgradeBlossom()));

    assert(m_interface->addBlossom("ini_file", "delete", new IniDeleteEntryBlossom()));
    assert(m_interface->addBlossom("ini_file", "read", new IniReadEntryBlossom()));
    assert(m_interface->addBlossom("ini_file", "set", new IniSetEntryBlossom()));

    assert(m_interface->addBlossom("path", "chmod", new PathChmodBlossom()));
    assert(m_interface->addBlossom("path", "chown", new PathChownBlossom()));
    assert(m_interface->addBlossom("path", "copy", new PathCopyBlossom()));
    assert(m_interface->addBlossom("path", "delete", new PathDeleteBlossom()));
    assert(m_interface->addBlossom("path", "rename", new PathRenameBlossom()));

    assert(m_interface->addBlossom("template", "create_string", new TemplateCreateStringBlossom()));
    assert(m_interface->addBlossom("template", "create_file", new TemplateCreateFileBlossom()));

    assert(m_interface->addBlossom("special", "assert", new AssertBlossom()));
    assert(m_interface->addBlossom("special", "cmd", new CmdBlossom()));
    assert(m_interface->addBlossom("special", "exit", new ExitBlossom()));
    assert(m_interface->addBlossom("special", "item_update", new ItemUpdateBlossom()));
    assert(m_interface->addBlossom("special", "print", new PrintBlossom()));

    assert(m_interface->addBlossom("text_file", "append", new TextAppendBlossom()));
    assert(m_interface->addBlossom("text_file", "read", new TextReadBlossom()));
    assert(m_interface->addBlossom("text_file", "replace", new TextReplaceBlossom()));
    assert(m_interface->addBlossom("text_file", "write", new TextWriteBlossom()));

    assert(m_interface->addBlossom("ssh", "file_create", new SshCmdCreateFileBlossom()));
    assert(m_interface->addBlossom("ssh", "scp", new SshScpBlossom()));
    assert(m_interface->addBlossom("ssh", "cmd", new SshCmdBlossom()));
}

/**
 * @brief execute a cli-command
 *
 * @param command cli-command to execute
 * @param errorMessage reference for error-message
 *
 * @return true, if successful, else false
 */
bool
SakuraRoot::runCommand(const std::string &command,
                       std::string &errorMessage)
{
    LOG_DEBUG("run command: " + command);

    // run command
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(command);
    if(processResult.success == false)
    {
        errorMessage = processResult.processOutput;
        return false;
    }

    return true;
}
