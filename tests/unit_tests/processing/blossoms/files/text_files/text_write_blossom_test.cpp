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
#include <processing/blossoms/files/text_files/text_write_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

TextWriteBlossom_Test::TextWriteBlossom_Test()
    : Kitsunemimi::CompareTestHelper("TextWriteBlossom_Test")
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

    Kitsunemimi::Sakura::BlossomItem fakeItem;
    runSyncProcess("rm " + m_path);
}

/**
 * @brief initTask_test
 */
void
TextWriteBlossom_Test::initTask_test()
{
    TextWriteBlossom writeBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;

    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initBlossom(blossomItem);
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
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    runSyncProcess("rm -r " + m_path);
    runSyncProcess("touch " + m_path);

    writeBlossom.preCheck(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
    TEST_EQUAL(blossomItem.skip, false);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief runTask_test
 */
void
TextWriteBlossom_Test::runTask_test()
{
    std::string errorMessage = "";

    TextWriteBlossom writeBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initBlossom(blossomItem);
    writeBlossom.runTask(blossomItem);
    TEST_EQUAL(blossomItem.success, true);

    std::string fileContent = "";
    const bool result = Kitsunemimi::Persistence::readFile(fileContent, m_path, errorMessage);

    TEST_EQUAL(result, true);
    TEST_EQUAL(fileContent, m_text);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief postCheck_test
 */
void
TextWriteBlossom_Test::postCheck_test()
{
    TextWriteBlossom writeBlossom;
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initBlossom(blossomItem);
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
    Kitsunemimi::Sakura::BlossomItem blossomItem;
    blossomItem.values.insert("file_path", new DataValue(m_path));
    blossomItem.values.insert("text", new DataValue(m_text));

    writeBlossom.initBlossom(blossomItem);
    writeBlossom.closeBlossom(blossomItem);
    TEST_EQUAL(blossomItem.success, true);
}
