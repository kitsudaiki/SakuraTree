/**
 * @file        text_read_blossom_test.cpp
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

#include "text_read_blossom_test.h"
#include <processing/process_execution.h>
#include <items/sakura_items.h>
#include <processing/blossoms/files/text_files/text_read_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TextReadBlossom_Test::TextReadBlossom_Test()
    : Kitsunemimi::Common::Test("TextReadBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

void
TextReadBlossom_Test::initTestCase()
{
    m_path = "/tmp/TextReadBlossom_Test_testfile";
    m_text = "this is a test-string";

    BlossomItem fakeItem;
    runSyncProcess(fakeItem, "rm " + m_path);
}

/**
 * @brief initTask_test
 */
void
TextReadBlossom_Test::initTask_test()
{
    TextReadBlossom readBlossom;
    BlossomItem blossomItem;

    readBlossom.initTask(blossomItem);
    TEST_EQUAL(blossomItem.success, false);

    blossomItem.groupValues.insert("file_path", new DataValue(m_path));

    readBlossom.initTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    TEST_EQUAL(readBlossom.m_filePath, m_path);
}

/**
 * @brief preCheck_test
 */
void
TextReadBlossom_Test::preCheck_test()
{
    TextReadBlossom readBlossom;
    BlossomItem blossomItem;
    blossomItem.groupValues.insert("file_path", new DataValue(m_path));

    readBlossom.initTask(blossomItem);
    readBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess(blossomItem, "mkdir " + m_path);

    readBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess(blossomItem, "rm -r " + m_path);
    runSyncProcess(blossomItem, "touch " + m_path);

    readBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess(blossomItem, "rm " + m_path);
}

/**
 * @brief runTask_test
 */
void
TextReadBlossom_Test::runTask_test()
{
    TextReadBlossom readBlossom;
    BlossomItem blossomItem;
    blossomItem.groupValues.insert("file_path", new DataValue(m_path));

    Kitsunemimi::Persistence::writeFile(m_path, m_text, true);

    readBlossom.initTask(blossomItem);
    readBlossom.runTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
    TEST_EQUAL(blossomItem.blossomOutput->toString(), m_text);

    runSyncProcess(blossomItem, "rm " + m_path);
}

/**
 * @brief postCheck_test
 */
void
TextReadBlossom_Test::postCheck_test()
{
    TextReadBlossom readBlossom;
    BlossomItem blossomItem;
    blossomItem.groupValues.insert("file_path", new DataValue(m_path));

    readBlossom.postCheck(blossomItem);
    readBlossom.closeTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

/**
 * @brief closeTask_test
 */
void
TextReadBlossom_Test::closeTask_test()
{
    TextReadBlossom readBlossom;
    BlossomItem blossomItem;
    blossomItem.groupValues.insert("file_path", new DataValue(m_path));

    readBlossom.initTask(blossomItem);
    readBlossom.closeTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

}
