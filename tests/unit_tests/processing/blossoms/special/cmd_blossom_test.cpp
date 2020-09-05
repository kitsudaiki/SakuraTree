/**
 * @file        template_create_blossom_test.cpp
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

#include "cmd_blossom_test.h"
#include <processing/blossoms/special/cmd_blossom.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

CmdBlossom_Test::CmdBlossom_Test()
    : Kitsunemimi::CompareTestHelper("CmdBlossom_Test")
{
    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

CmdBlossom_Test::~CmdBlossom_Test()
{
}

/**
 * @brief initTestCase
 */
void
CmdBlossom_Test::initTestCase()
{
    m_filePath = "/tmp/CmdBlossom_Test_file";
    m_command = "touch " + m_filePath;

    Kitsunemimi::Sakura::BlossomItem fakeItem;
    runSyncProcess("rm " + m_filePath);
}

/**
 * @brief initTask_test
 */
void
CmdBlossom_Test::initTask_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    CmdBlossom fakeCmdBlossom;

    fakeItem.values.insert("command", new DataValue(m_command));

    fakeCmdBlossom.initBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

/**
 * @brief preCheck_test
 */
void
CmdBlossom_Test::preCheck_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("command", new DataValue(m_command));
    CmdBlossom fakeCmdBlossom;

    fakeCmdBlossom.initBlossom(fakeItem);
    fakeCmdBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

/**
 * @brief runTask_test
 */
void
CmdBlossom_Test::runTask_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("command", new DataValue(m_command));
    CmdBlossom fakeCmdBlossom;

    TEST_EQUAL(bfs::exists(m_filePath),  false);

    fakeCmdBlossom.initBlossom(fakeItem);
    fakeCmdBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(bfs::exists(m_filePath),  true);

    runSyncProcess("rm " + m_filePath);
}

/**
 * @brief postCheck_test
 */
void
CmdBlossom_Test::postCheck_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("command", new DataValue(m_command));
    CmdBlossom fakeCmdBlossom;

    fakeCmdBlossom.initBlossom(fakeItem);
    fakeCmdBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

/**
 * @brief closeTask_test
 */
void
CmdBlossom_Test::closeTask_test()
{
    Kitsunemimi::Sakura::BlossomItem fakeItem;
    fakeItem.values.insert("command", new DataValue(m_command));
    CmdBlossom fakeCmdBlossom;

    fakeCmdBlossom.initBlossom(fakeItem);
    fakeCmdBlossom.closeBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}
