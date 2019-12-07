/**
 * @file        runUnitTests.cpp
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

#include "run_unit_tests.h"

#include <tests/items/item_methods_test.h>
#include <tests/items/value_items_functions_test.h>

#include <tests/processing/blossoms/files/file_methods_test.h>

#include <tests/processing/blossoms/files/common_files/file_chmod_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/file_chown_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/file_copy_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/file_delete_blossom_test.h>
#include <tests/processing/blossoms/files/common_files/file_rename_blossom_test.h>

#include <tests/processing/blossoms/files/template_files/template_create_blossom_test.h>
#include <tests/processing/blossoms/special/cmd_blossom_test.h>

#include <tests/processing/blossoms/files/text_files/text_append_blossom_test.h>
#include <tests/processing/blossoms/files/text_files/text_read_blossom_test.h>
#include <tests/processing/blossoms/files/text_files/text_replace_blossom_test.h>
#include <tests/processing/blossoms/files/text_files/text_write_blossom_test.h>

namespace SakuraTree
{

RunUnitTests::RunUnitTests()
{

}

void
RunUnitTests::run()
{
    #ifdef RUN_UNIT_TEST
    SakuraTree::ValueItemsFunctions_Test();
    SakuraTree::ItemMethods_Test();
    SakuraTree::FileMethods_Test();
    //SakuraTree::FileChmodBlossom_Test();
    //SakuraTree::FileChownBlossom_Test();
    SakuraTree::FileCopyBlossom_Test();
    SakuraTree::FileDeleteBlossom_Test();
    SakuraTree::FileRenameBlossom_Test();
    SakuraTree::TemplateCreateBlossom_Test();
    SakuraTree::CmdBlossom_Test();
    SakuraTree::TextReadBlossom_Test();
    SakuraTree::TextWriteBlossom_Test();
    SakuraTree::TextAppendBlossom_Test();
    SakuraTree::TextReplaceBlossom_Test();
    #endif
}

}
