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
#include <processing/blossoms/files/template_files/template_create_file_blossom.h>
#include <libKitsunemimiPersistence/files/text_file.h>

TemplateCreateBlossom_Test::TemplateCreateBlossom_Test()
    : Kitsunemimi::CompareTestHelper("TemplateCreateBlossom_Test")
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
    runSyncProcess("mkdir /tmp/templates");
    runSyncProcess("rm " + m_templatePath);
    runSyncProcess("rm " + m_destinationFile);
}

/**
 * @brief initTask_test
 */
void
TemplateCreateBlossom_Test::initTask_test()
{
    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    TemplateCreateFileBlossom fakeCreateBlossom;

    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));

    fakeCreateBlossom.initBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    TEST_EQUAL(fakeCreateBlossom.m_templatePath, m_templatePath);
    TEST_EQUAL(fakeCreateBlossom.m_destinationPath, m_destinationFile);

    runSyncProcess("rm " + m_templatePath);
    runSyncProcess("rm " + m_destinationFile);
}

/**
 * @brief preCheck_test
 */
void
TemplateCreateBlossom_Test::preCheck_test()
{
    std::string errorMessage = "";

    BlossomItem fakeItem;
    fakeItem.blossomPath = "/tmp/";
    fakeItem.values.insert("source_path", new DataValue(m_localTemplatePath));
    fakeItem.values.insert("dest_path", new DataValue(m_destinationFile));
    TemplateCreateFileBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    Kitsunemimi::Persistence::writeFile(m_templatePath, "poi {{asdf}}", errorMessage, true);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, false);
    TEST_EQUAL(fakeItem.skip, false);

    fakeItem.values.insert("asdf", new DataValue("poi"));

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    Kitsunemimi::Persistence::writeFile(m_destinationFile, "poi", errorMessage, true);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, false);

    Kitsunemimi::Persistence::writeFile(m_destinationFile, "poi poi", errorMessage, true);

    fakeCopyBlossom.preCheck(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
    TEST_EQUAL(fakeItem.skip, true);

    runSyncProcess("rm " + m_destinationFile);
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
    TemplateCreateFileBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess("rm " + m_templatePath);
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
    TemplateCreateFileBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.runTask(fakeItem);
    TEST_EQUAL(fakeItem.success, true);

    runSyncProcess("rm " + m_destinationFile);

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
    TemplateCreateFileBlossom fakeCopyBlossom;

    fakeCopyBlossom.initBlossom(fakeItem);
    fakeCopyBlossom.closeBlossom(fakeItem);
    TEST_EQUAL(fakeItem.success, true);
}
