/**
 * @file        path_copy_blossom_test.cpp
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

#include "path_copy_blossom_test.h"
#include <processing/blossoms/files/common_files/path_copy_blossom.h>

PathCopyBlossom_Test::PathCopyBlossom_Test()
    : Kitsunemimi::CompareTestHelper("PathCopyBlossom_Test")
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
PathCopyBlossom_Test::initTestCase()
{
    m_sourceFile = "/tmp/FileCopyBlossom_Test_testfile_source";
    m_destinationFile = "/tmp/FileCopyBlossom_Test_testfile_destination";

    Kitsunemimi::Sakura::BlossomItem fakeItem;
    runSyncProcess("rm " + m_sourceFile);
    runSyncProcess("rm " + m_destinationFile);
}

/**
 * @brief initTask_test
 */
void
PathCopyBlossom_Test::initTask_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    PathCopyBlossom fakeCopyBlossom;

    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    fakeCopyBlossom.initBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    TEST_EQUAL(fakeCopyBlossom.m_sourcePath, m_sourceFile);
    TEST_EQUAL(fakeCopyBlossom.m_destinationPath, m_destinationFile);
}

/**
 * @brief preCheck_test
 */
void
PathCopyBlossom_Test::preCheck_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    PathCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess("touch " + m_sourceFile);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess("touch " + m_destinationFile);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    runSyncProcess("rm " + m_destinationFile);
}

/**
 * @brief runTask_test
 */
void
PathCopyBlossom_Test::runTask_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    PathCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess("rm " + m_sourceFile);

    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief postCheck_test
 */
void
PathCopyBlossom_Test::postCheck_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    PathCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    runSyncProcess("touch " + m_destinationFile);
    fakeCopyBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess("rm " + m_destinationFile);

    fakeCopyBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief closeTask_test
 */
void
PathCopyBlossom_Test::closeTask_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("source_path", new DataValue(m_sourceFile));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    PathCopyBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.closeBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}
