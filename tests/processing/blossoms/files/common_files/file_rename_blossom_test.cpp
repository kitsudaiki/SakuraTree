/**
 * @file        file_rename_blossom_test.cpp
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

#include "file_rename_blossom_test.h"
#include <processing/common/process_execution.h>
#include <items/sakura_items.h>
#include <processing/blossoms/files/common_files/file_rename_blossom.h>

namespace SakuraTree
{

FileRenameBlossom_Test::FileRenameBlossom_Test()
    : Kitsunemimi::Common::Test("FileRenameBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

/**
 * @brief initTestCase
 */
void
FileRenameBlossom_Test::initTestCase()
{
    m_sourceFile = "/tmp/FileRenameBlossom_Test_testfile_source";
    m_destinationFileName = "FileRenameBlossom_Test_testfile_destination";
    m_destinationFile = "/tmp/FileRenameBlossom_Test_testfile_destination";

    BlossomItem fakeItem;
    runSyncProcess(fakeItem, "rm " + m_sourceFile);
    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief initTask_test
 */
void
FileRenameBlossom_Test::initTask_test()
{
    BlossomItem fakeItem;
    FileRenameBlossom fakeRenameBlossom;

    fakeRenameBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    fakeItem.values.insert("file_path", new DataValue(m_sourceFile));

    fakeRenameBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    fakeItem.values.insert("new_name", new DataValue(m_destinationFileName));

    fakeRenameBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    TEST_EQUAL(fakeRenameBlossom.m_filePath, m_sourceFile);
    TEST_EQUAL(fakeRenameBlossom.m_newFilePath, m_destinationFile);
}

/**
 * @brief preCheck_test
 */
void
FileRenameBlossom_Test::preCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("new_name", new DataValue(m_destinationFileName));

    FileRenameBlossom fakeRenameBlossom;

    fakeRenameBlossom.initTask(fakeItem);
    fakeRenameBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess(fakeItem, "touch " + m_sourceFile);

    fakeRenameBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess(fakeItem, "touch " + m_destinationFile);
    fakeRenameBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess(fakeItem, "rm " + m_sourceFile);
    fakeRenameBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, true);

    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief runTask_test
 */
void
FileRenameBlossom_Test::runTask_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("new_name", new DataValue(m_destinationFileName));

    FileRenameBlossom fakeRenameBlossom;

    runSyncProcess(fakeItem, "touch " + m_sourceFile);
    runSyncProcess(fakeItem, "rm " + m_destinationFile);

    fakeRenameBlossom.initTask(fakeItem);
    fakeRenameBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess(fakeItem, "rm " + m_sourceFile);

    fakeRenameBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief postCheck_test
 */
void
FileRenameBlossom_Test::postCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("new_name", new DataValue(m_destinationFileName));

    FileRenameBlossom fakeRenameBlossom;

    fakeRenameBlossom.initTask(fakeItem);
    fakeRenameBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess(fakeItem, "touch " + m_sourceFile);
    fakeRenameBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    runSyncProcess(fakeItem, "rm " + m_sourceFile);
    runSyncProcess(fakeItem, "rm " + m_destinationFile);

    fakeRenameBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief closeTask_test
 */
void
FileRenameBlossom_Test::closeTask_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("new_name", new DataValue(m_destinationFileName));

    FileRenameBlossom fakeRenameBlossom;

    fakeRenameBlossom.initTask(fakeItem);
    fakeRenameBlossom.closeTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

}
