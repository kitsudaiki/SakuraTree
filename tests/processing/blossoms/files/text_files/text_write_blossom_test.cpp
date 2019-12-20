/**
 * @file        text_write_blossom_test.cpp
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

#include "text_write_blossom_test.h"
#include <processing/common/process_execution.h>
#include <items/sakura_items.h>
#include <processing/blossoms/files/text_files/text_write_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TextWriteBlossom_Test::TextWriteBlossom_Test()
    : Kitsunemimi::Common::Test("TextWriteBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

void
TextWriteBlossom_Test::initTestCase()
{
    m_path = "/tmp/TextWriteBlossom_Test_testfile";
    m_text = "this is a test-string";

    BlossomItem fakeItem;
    runSyncProcess(fakeItem, "rm " + m_path);
}

/**
 * @brief initTask_test
 */
void
TextWriteBlossom_Test::initTask_test()
{
    TextWriteBlossom writeBlossom;
    BlossomItem blossomItem;

    writeBlossom.initTask(blossomItem);
    TEST_EQUAL(blossomItem.success, false);

    blossomItem.values.insert("file_path", new DataValue(m_path));

    writeBlossom.initTask(blossomItem);
    TEST_EQUAL(blossomItem.success, false);

    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    TEST_EQUAL(writeBlossom.m_filePath, m_path);
    TEST_EQUAL(writeBlossom.m_text, m_text);
}

/**
 * @brief preCheck_test
 */
void
TextWriteBlossom_Test::preCheck_test()
{
    TextWriteBlossom writeBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initTask(blossomItem);
    writeBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess(blossomItem, "mkdir " + m_path);

    writeBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess(blossomItem, "rm -r " + m_path);
    runSyncProcess(blossomItem, "touch " + m_path);

    writeBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess(blossomItem, "rm " + m_path);
}

/**
 * @brief runTask_test
 */
void
TextWriteBlossom_Test::runTask_test()
{
    TextWriteBlossom writeBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initTask(blossomItem);
    writeBlossom.runTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    const std::pair<bool, std::string> result = Kitsunemimi::Persistence::readFile(m_path);
    TEST_EQUAL(result.first, true);
    TEST_EQUAL(result.second, m_text);

    runSyncProcess(blossomItem, "rm " + m_path);
}

/**
 * @brief postCheck_test
 */
void
TextWriteBlossom_Test::postCheck_test()
{
    TextWriteBlossom writeBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initTask(blossomItem);
    writeBlossom.postCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

/**
 * @brief closeTask_test
 */
void
TextWriteBlossom_Test::closeTask_test()
{
    TextWriteBlossom writeBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initTask(blossomItem);
    writeBlossom.closeTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

}
