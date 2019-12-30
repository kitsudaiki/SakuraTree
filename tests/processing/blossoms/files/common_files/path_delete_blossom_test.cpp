/**
 * @file        path_delete_blossom_test.cpp
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

#include "path_delete_blossom_test.h"
#include <items/sakura_items.h>
#include <processing/blossoms/files/common_files/path_delete_blossom.h>

namespace SakuraTree
{

PathDeleteBlossom_Test::PathDeleteBlossom_Test()
    : Kitsunemimi::Common::Test("FileDeleteBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

void
PathDeleteBlossom_Test::initTestCase()
{
    m_path = "/tmp/FileDeleteBlossom_Test_testfile";

    BlossomItem fakeItem;
    runSyncProcess("rm " + m_path);
}

/**
 * @brief initTask_test
 */
void
PathDeleteBlossom_Test::initTask_test()
{
    BlossomItem fakeItem;
    PathDeleteBlossom fakeDeleteBlossom;

    fakeItem.values.insert("file_path", new DataValue(m_path));

    fakeDeleteBlossom.initBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    TEST_EQUAL(fakeDeleteBlossom.m_filePath, m_path);
}

/**
 * @brief preCheck_test
 */
void
PathDeleteBlossom_Test::preCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_path));

    PathDeleteBlossom fakeDeleteBlossom;

    fakeDeleteBlossom.initBlossom(fakeItem);
    fakeDeleteBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    runSyncProcess("touch " + m_path);

    fakeDeleteBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief runTask_test
 */
void
PathDeleteBlossom_Test::runTask_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_path));

    PathDeleteBlossom fakeDeleteBlossom;

    fakeDeleteBlossom.initBlossom(fakeItem);
    fakeDeleteBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    runSyncProcess("touch " + m_path);

    fakeDeleteBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    fakeDeleteBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief postCheck_test
 */
void
PathDeleteBlossom_Test::postCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_path));

    PathDeleteBlossom fakeDeleteBlossom;

    fakeDeleteBlossom.initBlossom(fakeItem);
    fakeDeleteBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess("touch " + m_path);
    fakeDeleteBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    runSyncProcess("rm " + m_path);
}

/**
 * @brief closeTask_test
 */
void
PathDeleteBlossom_Test::closeTask_test()
{
    BlossomItem fakeItem;
    fakeItem.values.insert("file_path", new DataValue(m_path));

    PathDeleteBlossom fakeDeleteBlossom;

    fakeDeleteBlossom.initBlossom(fakeItem);
    fakeDeleteBlossom.closeBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

}
