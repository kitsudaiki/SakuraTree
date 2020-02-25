/**
 * @file        text_append_blossom_test.cpp
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

#include "text_append_blossom_test.h"
#include <items/sakura_items.h>
#include <processing/blossoms/files/text_files/text_append_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TextAppendBlossom_Test::TextAppendBlossom_Test()
    : Kitsunemimi::CompareTestHelper("TextAppendBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

void
TextAppendBlossom_Test::initTestCase()
{
    m_path = "/tmp/TextAppendBlossom_Test_testfile";
    m_text = "this is a test-string";
    m_newText = "!!!";

    BlossomItem fakeItem;
    runSyncProcess("rm " + m_path);
}

/**
 * @brief initTask_test
 */
void
TextAppendBlossom_Test::initTask_test()
{
    TextAppendBlossom appendBlossom;
    BlossomItem blossomItem;

    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_newText));

    appendBlossom.initBlossom(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    TEST_EQUAL(appendBlossom.m_filePath, m_path);
    TEST_EQUAL(appendBlossom.m_newText, m_newText);
}

/**
 * @brief preCheck_test
 */
void
TextAppendBlossom_Test::preCheck_test()
{
    TextAppendBlossom appendBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_newText));

    appendBlossom.initBlossom(blossomItem);
    appendBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("mkdir " + m_path);

    appendBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("rm -r " + m_path);
    runSyncProcess("touch " + m_path);

    appendBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief runTask_test
 */
void
TextAppendBlossom_Test::runTask_test()
{
    std::string errorMessage = "";

    TextAppendBlossom appendBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_newText));

    Kitsunemimi::Persistence::writeFile(m_path, m_text, errorMessage, true);

    appendBlossom.initBlossom(blossomItem);
    appendBlossom.runTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    const std::pair<bool, std::string> result = Kitsunemimi::Persistence::readFile(m_path,
                                                                                   errorMessage);
    TEST_EQUAL(result.first, true);
    TEST_EQUAL(result.second, m_text + m_newText);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief postCheck_test
 */
void
TextAppendBlossom_Test::postCheck_test()
{
    TextAppendBlossom appendBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_newText));

    appendBlossom.initBlossom(blossomItem);
    appendBlossom.postCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

/**
 * @brief closeTask_test
 */
void
TextAppendBlossom_Test::closeTask_test()
{
    TextAppendBlossom appendBlossom;
    BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_newText));

    appendBlossom.initBlossom(blossomItem);
    appendBlossom.closeBlossom(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

}
