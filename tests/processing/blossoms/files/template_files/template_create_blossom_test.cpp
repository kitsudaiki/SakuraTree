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

#include "template_create_blossom_test.h"
#include <processing/common/process_execution.h>
#include <items/sakura_items.h>
#include <processing/blossoms/files/template_files/template_create_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TemplateCreateBlossom_Test::TemplateCreateBlossom_Test()
    : Kitsunemimi::Common::Test("TemplateCreateBlossom_Test")
{
    m_root = new SakuraRoot(std::string("test"));

    initTestCase();
    initTask_test();
    preCheck_test();
    runTask_test();
    postCheck_test();
    closeTask_test();
}

TemplateCreateBlossom_Test::~TemplateCreateBlossom_Test()
{
    delete m_root;
}

/**
 * @brief initTestCase
 */
void
TemplateCreateBlossom_Test::initTestCase()
{
    m_templatePath = "/tmp/templates/TemplateCreateBlossom_Test_testfile_source";
    m_localTemplatePath = "TemplateCreateBlossom_Test_testfile_source";
    m_destinationFile = "/tmp/TemplateCreateBlossom_Test_testfile_destination";

    BlossomItem fakeItem;
    runSyncProcess(fakeItem, "mkdir /tmp/templates");
    runSyncProcess(fakeItem, "rm " + m_templatePath);
    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief initTask_test
 */
void
TemplateCreateBlossom_Test::initTask_test()
{
    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    TemplateCreateBlossom fakeCreateBlossom;

    fakeCreateBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));

    fakeCreateBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, false);

    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    fakeCreateBlossom.initTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    TEST_EQUAL(fakeCreateBlossom.m_templatePath, m_templatePath);
    TEST_EQUAL(fakeCreateBlossom.m_destinationPath, m_destinationFile);

    runSyncProcess(fakeItem, "rm " + m_templatePath);
    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief preCheck_test
 */
void
TemplateCreateBlossom_Test::preCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));
    TemplateCreateBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    Kitsunemimi::Persistence::writeFile(m_templatePath, "poi {{asdf}}", true);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    fakeItem.values.insert("asdf", new DataValue("poi"));

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    Kitsunemimi::Persistence::writeFile(m_destinationFile, "poi", true);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    Kitsunemimi::Persistence::writeFile(m_destinationFile, "poi poi", true);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, true);

    runSyncProcess(fakeItem, "rm " + m_destinationFile);
}

/**
 * @brief runTask_test
 */
void
TemplateCreateBlossom_Test::runTask_test()
{
    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));
    TemplateCreateBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess(fakeItem, "rm " + m_templatePath);
}

/**
 * @brief postCheck_test
 */
void
TemplateCreateBlossom_Test::postCheck_test()
{
    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));
    TemplateCreateBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess(fakeItem, "rm " + m_destinationFile);

    fakeCopyBlossom.postCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
}

/**
 * @brief closeTask_test
 */
void
TemplateCreateBlossom_Test::closeTask_test()
{
    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));
    TemplateCreateBlossom fakeCopyBlossom;

    fakeCopyBlossom.initTask(fakeItem);
    fakeCopyBlossom.closeTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}

}
