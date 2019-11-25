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

#include <processing/blossoms/files/common_files/file_copy_blossom.h>
#include <processing/blossoms/files/common_files/file_delete_blossom.h>
#include <processing/blossoms/files/common_files/file_rename_blossom.h>

#include <processing/blossoms/files/template_files/template_create_blossom.h>

namespace SakuraTree
{

/**
 * request a new blossom-object to process the requested task
 *
 * @return pointer to a new object or nullptr if type or subtype is unknown
 */
Blossom*
getBlossom(const std::string type,
           const std::string subType)
{
    if(type == "apt")
    {
        if(subType == "absent") {
            return new AptAbsentBlossom();
        }
        if(subType == "latest") {
            return new AptLatestBlossom();
        }
        if(subType == "present") {
            return new AptPresentBlossom();
        }
        if(subType == "update") {
            return new AptUdateBlossom();
        }
        if(subType == "upgrade") {
            return new AptUpgradeBlossom();
        }
    }

    if(type == "ssh")
    {
        if(subType == "cmd") {
            return new SshCmdBlossom();
        }
        if(subType == "scp") {
            return new SshScpBlossom();
        }
        if(subType == "file_create") {
            return new SshCmdCreateFileBlossom();
        }
    }

    if(type == "special")
    {
        if(subType == "copy-subtree") {
            return new SakuraCopySubtreeBlossom();
        }
        if(subType == "print") {
            return new PrintBlossom();
        }
        if(subType == "cmd") {
            return new CmdBlossom();
        }
    }

    if(type == "template")
    {
        if(subType == "create") {
            return new TemplateCreateBlossom();
        }
    }

    if(type == "file")
    {
        if(subType == "copy") {
            return new FileCopyBlossom();
        }
        if(subType == "move") {
            return new FileRenameBlossom();
        }
        if(subType == "delete") {
            return new FileDeleteBlossom();
        }
    }


    assert(false);

    return nullptr;
}

}
