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
#include <config.h>

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>

#include <libKitsunemimiConfig/config_handler.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

#include <blossoms/install/apt/apt_absent_blossom.h>
#include <blossoms/install/apt/apt_latest_blossom.h>
#include <blossoms/install/apt/apt_present_blossom.h>
#include <blossoms/install/apt/apt_update_blossom.h>
#include <blossoms/install/apt/apt_upgrade_blossom.h>

#include <blossoms/ssh/ssh_cmd_blossom.h>
#include <blossoms/ssh/ssh_scp_blossom.h>
#include <blossoms/ssh/ssh_cmd_create_file_blossom.h>

#include <blossoms/special/print_blossom.h>
#include <blossoms/special/cmd_blossom.h>
#include <blossoms/special/assert_blossom.h>
#include <blossoms/special/exit_blossom.h>
#include <blossoms/special/item_update_blossom.h>

#include <blossoms/files/common_files/path_copy_blossom.h>
#include <blossoms/files/common_files/path_delete_blossom.h>
#include <blossoms/files/common_files/path_rename_blossom.h>
#include <blossoms/files/common_files/path_chmod_blossom.h>
#include <blossoms/files/common_files/path_chown_blossom.h>

#include <blossoms/files/template_files/template_create_file_blossom.h>
#include <blossoms/files/template_files/template_create_string_blossom.h>

#include <blossoms/files/text_files/text_append_blossom.h>
#include <blossoms/files/text_files/text_read_blossom.h>
#include <blossoms/files/text_files/text_replace_blossom.h>
#include <blossoms/files/text_files/text_write_blossom.h>

#include <blossoms/files/ini_files/ini_delete_entry_blossom.h>
#include <blossoms/files/ini_files/ini_read_entry_blossom.h>
#include <blossoms/files/ini_files/ini_set_entry_blossom.h>

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
 * @param initialTreePath
 * @param seedPath
 * @param initialValues
 * @param serverAddress
 * @param port
 *
 * @return true if successful, else false
 */
bool
SakuraRoot::startProcess(const std::string &inputPath,
                         const DataMap &initialValues,
                         const bool dryRun)
{
    initBlossoms();

    // set default-file in case that a directory instead of a file was selected
    std::string treeFile = inputPath;
    if(bfs::is_directory(treeFile)) {
        treeFile = treeFile + "/root.sakura";
    }

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
 * @brief SakuraRoot::initBlossoms
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
