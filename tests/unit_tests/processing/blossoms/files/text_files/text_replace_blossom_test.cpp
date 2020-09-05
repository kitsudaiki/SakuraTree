/**
 * @file        text_replace_blossom_test.cpp
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

#include "text_replace_blossom_test.h"
#include <processing/blossoms/files/text_files/text_replace_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

TextReplaceBlossom_Test::TextReplaceBlossom_Test()
    : Kitsunemimi::CompareTestHelper("TextReplaceBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

void
TextReplaceBlossom_Test::initTestCase()
{
    m_path = "/tmp/TextReplaceBlossom_Test_testfile";
    m_oldCompleteText = "this is a test-string";
    m_newCompleteText = "this is a fake-string";
    m_oldText = "test";
    m_newText = "fake";

    Kitsunemimi::Sakura::BlossomItem fakeItem;
    runSyncProcess("rm " + m_path);
}

/**
 * @brief initTask_test
 */
void
TextReplaceBlossom_Test::initTask_test()
{
    TextReplaceBlossom replaceBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;

    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("old_text", new DataValue(m_oldText));
    blossomItem.values.insert("new_text", new DataValue(m_newText));

    replaceBlossom.initBlossom(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    TEST_EQUAL(replaceBlossom.m_filePath, m_path);
    TEST_EQUAL(replaceBlossom.m_oldText, m_oldText);
    TEST_EQUAL(replaceBlossom.m_newText, m_newText);
}

/**
 * @brief preCheck_test
 */
void
TextReplaceBlossom_Test::preCheck_test()
{
    TextReplaceBlossom replaceBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("old_text", new DataValue(m_oldText));
    blossomItem.values.insert("new_text", new DataValue(m_newText));

    replaceBlossom.initBlossom(blossomItem);
    replaceBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("mkdir " + m_path);

    replaceBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, false);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("rm -r " + m_path);
    runSyncProcess("touch " + m_path);

    replaceBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief runTask_test
 */
void
TextReplaceBlossom_Test::runTask_test()
{
    std::string errorMessage = "";

    TextReplaceBlossom replaceBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("old_text", new DataValue(m_oldText));
    blossomItem.values.insert("new_text", new DataValue(m_newText));

    Kitsunemimi::Persistence::writeFile(m_path, m_oldCompleteText, errorMessage, true);

    replaceBlossom.initBlossom(blossomItem);
    replaceBlossom.runTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    std::string fileContent = "";
    const bool result = Kitsunemimi::Persistence::readFile(fileContent, m_path, errorMessage);

    TEST_EQUAL(result, true);
    TEST_EQUAL(fileContent, m_newCompleteText);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief postCheck_test
 */
void
TextReplaceBlossom_Test::postCheck_test()
{
    TextReplaceBlossom replaceBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("old_text", new DataValue(m_oldText));
    blossomItem.values.insert("new_text", new DataValue(m_newText));

    replaceBlossom.initBlossom(blossomItem);
    replaceBlossom.postCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}

/**
 * @brief closeTask_test
 */
void
TextReplaceBlossom_Test::closeTask_test()
{
    TextReplaceBlossom replaceBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("old_text", new DataValue(m_oldText));
    blossomItem.values.insert("new_text", new DataValue(m_newText));

    replaceBlossom.initBlossom(blossomItem);
    replaceBlossom.closeBlossom(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}
