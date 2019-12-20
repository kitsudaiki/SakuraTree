/**
 * @file        file_copy_blossom_test.cpp
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

#include "file_copy_blossom_test.h"
#include <processing/common/process_execution.h>
#include <items/sakura_items.h>
#include <processing/blossoms/files/common_files/file_copy_blossom.h>

namespace SakuraTree
{

FileCopyBlossom_Test::FileCopyBlossom_Test()
    : Kitsunemimi::Common::Test("FileCopyBlossom_Test")
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
FileCopyBlossom_Test::initTestCase()
{
    m_sourceFile = "/tmp/FileCopyBlossom_Test_testfile_source";
    m_destinationFile = "/tmp/FileCopyBlossom_Test_testfile_destination";

    BlossomItem fakeItem;
    runSyncProcess(fakeItem, "rm " + m_sourceFile);
    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief initTask_test
 */
void
FileCopyBlossom_Test::initTask_test()
{
    BlossomItem fakeItem;
    FileCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));

    fakeCopyBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    fakeCopyBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    TEST_EQUAL(fakeCopyBlossom.m_sourcePath, m_sourceFile);
    TEST_EQUAL(fakeCopyBlossom.m_destinationPath, m_destinationFile);
}

/**
 * @brief preCheck_test
 */
void
FileCopyBlossom_Test::preCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    FileCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess(fakeItem, "touch " + m_sourceFile);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess(fakeItem, "touch " + m_destinationFile);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief runTask_test
 */
void
FileCopyBlossom_Test::runTask_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    FileCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess(fakeItem, "rm " + m_sourceFile);

    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief postCheck_test
 */
void
FileCopyBlossom_Test::postCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    FileCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    runSyncProcess(fakeItem, "touch " + m_destinationFile);
    fakeCopyBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess(fakeItem, "rm " + m_destinationFile);

    fakeCopyBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief closeTask_test
 */
void
FileCopyBlossom_Test::closeTask_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    FileCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.closeTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

}
