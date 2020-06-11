/**
 * @file        main_tests.cpp
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

#include <tests/processing/common/item_methods_test.h>
#include <tests/processing/common/functions_test.h>

#include <tests/processing/blossoms/files/common_files/path_chmod_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/path_chown_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/path_copy_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/path_delete_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/path_rename_blossom_test.h>

#include <tests/processing/blossoms/files/template_files/template_create_blossom_test.h>
#include <tests/processing/blossoms/special/cmd_blossom_test.h>

#include <tests/processing/blossoms/files/text_files/text_append_blossom_test.h>
#include <tests/processing/blossoms/files/text_files/text_read_blossom_test.h>
#include <tests/processing/blossoms/files/text_files/text_replace_blossom_test.h>
#include <tests/processing/blossoms/files/text_files/text_write_blossom_test.h>

int
main(int argc, char *argv[])
{
    SakuraTree::ValueItemsFunctions_Test();
    SakuraTree::ItemMethods_Test();
    //SakuraTree::PathChmodBlossom_Test();
    //SakuraTree::PathChownBlossom_Test();
    SakuraTree::PathCopyBlossom_Test();
    SakuraTree::PathDeleteBlossom_Test();
    SakuraTree::PathRenameBlossom_Test();
    SakuraTree::TemplateCreateBlossom_Test();
    SakuraTree::CmdBlossom_Test();
    SakuraTree::TextReadBlossom_Test();
    SakuraTree::TextWriteBlossom_Test();
    SakuraTree::TextAppendBlossom_Test();
    SakuraTree::TextReplaceBlossom_Test();
    return 0;
}
