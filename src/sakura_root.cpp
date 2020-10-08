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

    // parse and check input
    std::string errorMessage = "";
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();
    bool result = interface->readFiles(treeFile, errorMessage);
    if(result == false)
    {
        LOG_ERROR(errorMessage);
        return false;
    }

    // check if only dry-run
    if(dryRun) {
        return true;
    }

    // run task
    const bfs::path relativePath = bfs::path(treeFile).leaf();
    result = interface->triggerTree(relativePath.string(), initialValues, errorMessage);
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
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();

    assert(interface->addBlossom("apt", "absent", new AptAbsentBlossom()));
    assert(interface->addBlossom("apt", "latest", new AptLatestBlossom()));
    assert(interface->addBlossom("apt", "present", new AptPresentBlossom()));
    assert(interface->addBlossom("apt", "update", new AptUdateBlossom()));
    assert(interface->addBlossom("apt", "upgrade", new AptUpgradeBlossom()));

    assert(interface->addBlossom("ini_file", "delete", new IniDeleteEntryBlossom()));
    assert(interface->addBlossom("ini_file", "read", new IniReadEntryBlossom()));
    assert(interface->addBlossom("ini_file", "set", new IniSetEntryBlossom()));

    assert(interface->addBlossom("path", "chmod", new PathChmodBlossom()));
    assert(interface->addBlossom("path", "chown", new PathChownBlossom()));
    assert(interface->addBlossom("path", "copy", new PathCopyBlossom()));
    assert(interface->addBlossom("path", "delete", new PathDeleteBlossom()));
    assert(interface->addBlossom("path", "rename", new PathRenameBlossom()));

    assert(interface->addBlossom("template", "create_string", new TemplateCreateStringBlossom()));
    assert(interface->addBlossom("template", "create_file", new TemplateCreateFileBlossom()));

    assert(interface->addBlossom("special", "assert", new AssertBlossom()));
    assert(interface->addBlossom("special", "cmd", new CmdBlossom()));
    assert(interface->addBlossom("special", "exit", new ExitBlossom()));
    assert(interface->addBlossom("special", "item_update", new ItemUpdateBlossom()));
    assert(interface->addBlossom("special", "print", new PrintBlossom()));

    assert(interface->addBlossom("text_file", "append", new TextAppendBlossom()));
    assert(interface->addBlossom("text_file", "read", new TextReadBlossom()));
    assert(interface->addBlossom("text_file", "replace", new TextReplaceBlossom()));
    assert(interface->addBlossom("text_file", "write", new TextWriteBlossom()));

    assert(interface->addBlossom("ssh", "file_create", new SshCmdCreateFileBlossom()));
    assert(interface->addBlossom("ssh", "scp", new SshScpBlossom()));
    assert(interface->addBlossom("ssh", "cmd", new SshCmdBlossom()));
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
