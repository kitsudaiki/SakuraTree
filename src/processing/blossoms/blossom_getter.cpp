/**
 * @file        blossom_getter.cpp
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

#include "blossom_getter.h"
#include <processing/blossoms/blossom.h>

#include <processing/blossoms/install/apt/apt_absent_blossom.h>
#include <processing/blossoms/install/apt/apt_latest_blossom.h>
#include <processing/blossoms/install/apt/apt_present_blossom.h>
#include <processing/blossoms/install/apt/apt_update_blossom.h>
#include <processing/blossoms/install/apt/apt_upgrade_blossom.h>

#include <processing/blossoms/ssh/ssh_cmd_blossom.h>
#include <processing/blossoms/ssh/ssh_scp_blossom.h>
#include <processing/blossoms/ssh/ssh_cmd_create_file_blossom.h>

#include <processing/blossoms/special/sakura_copy_subtree_blossom.h>
#include <processing/blossoms/special/print_blossom.h>
#include <processing/blossoms/special/cmd_blossom.h>
#include <processing/blossoms/special/assert_blossom.h>
#include <processing/blossoms/special/exit_blossom.h>
#include <processing/blossoms/special/item_update_blossom.h>

#include <processing/blossoms/files/common_files/path_copy_blossom.h>
#include <processing/blossoms/files/common_files/path_delete_blossom.h>
#include <processing/blossoms/files/common_files/path_rename_blossom.h>
#include <processing/blossoms/files/common_files/path_chmod_blossom.h>
#include <processing/blossoms/files/common_files/path_chown_blossom.h>

#include <processing/blossoms/files/template_files/template_create_blossom.h>

#include <processing/blossoms/files/text_files/text_append_blossom.h>
#include <processing/blossoms/files/text_files/text_read_blossom.h>
#include <processing/blossoms/files/text_files/text_replace_blossom.h>
#include <processing/blossoms/files/text_files/text_write_blossom.h>

#include <processing/blossoms/files/ini_files/ini_delete_entry_blossom.h>
#include <processing/blossoms/files/ini_files/ini_read_entry_blossom.h>
#include <processing/blossoms/files/ini_files/ini_set_entry_blossom.h>

namespace SakuraTree
{

/**
 * request a new blossom-object to process the requested task
 *
 * @return pointer to a new object or nullptr if type or subtype is unknown
 */
Blossom*
getBlossom(const std::string blossomGroupType,
           const std::string blossomType)
{
    if(blossomGroupType == "apt")
    {
        if(blossomType == "absent") {
            return new AptAbsentBlossom();
        }
        if(blossomType == "latest") {
            return new AptLatestBlossom();
        }
        if(blossomType == "present") {
            return new AptPresentBlossom();
        }
        if(blossomType == "update") {
            return new AptUdateBlossom();
        }
        if(blossomType == "upgrade") {
            return new AptUpgradeBlossom();
        }
    }

    if(blossomGroupType == "ssh")
    {
        if(blossomType == "cmd") {
            return new SshCmdBlossom();
        }
        if(blossomType == "scp") {
            return new SshScpBlossom();
        }
        if(blossomType == "file_create") {
            return new SshCmdCreateFileBlossom();
        }
    }

    if(blossomGroupType == "special")
    {
        if(blossomType == "item_update") {
            return new ItemUpdateBlossom();
        }
        if(blossomType == "copy-subtree") {
            return new SakuraCopySubtreeBlossom();
        }
        if(blossomType == "print") {
            return new PrintBlossom();
        }
        if(blossomType == "cmd") {
            return new CmdBlossom();
        }
        if(blossomType == "assert") {
            return new AssertBlossom();
        }
        if(blossomType == "exit") {
            return new ExitBlossom();
        }
    }

    if(blossomGroupType == "template")
    {
        if(blossomType == "create") {
            return new TemplateCreateBlossom();
        }
    }

    if(blossomGroupType == "path")
    {
        if(blossomType == "chmod") {
            return new PathChmodBlossom();
        }
        if(blossomType == "chown") {
            return new PathChownBlossom();
        }
        if(blossomType == "copy") {
            return new PathCopyBlossom();
        }
        if(blossomType == "move") {
            return new PathRenameBlossom();
        }
        if(blossomType == "delete") {
            return new PathDeleteBlossom();
        }
    }

    if(blossomGroupType == "text_file")
    {
        if(blossomType == "read") {
            return new TextReadBlossom();
        }
        if(blossomType == "write") {
            return new TextWriteBlossom();
        }
        if(blossomType == "replace") {
            return new TextReplaceBlossom();
        }
        if(blossomType == "append") {
            return new TextAppendBlossom();
        }
    }

    if(blossomGroupType == "ini_file")
    {
        if(blossomType == "read") {
            return new IniReadEntryBlossom();
        }
        if(blossomType == "set") {
            return new IniSetEntryBlossom();
        }
        if(blossomType == "delete") {
            return new IniDeleteEntryBlossom();
        }
    }

    return nullptr;
}

}
