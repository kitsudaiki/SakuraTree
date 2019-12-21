/**
 * @file        file_methods_test.cpp
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

#include "file_methods_test.h"
#include <processing/blossoms/files/file_methods.h>
#include <processing/common/process_execution.h>

namespace SakuraTree
{

/**
 * @brief constructor
 */
FileMethods_Test::FileMethods_Test()
    : Kitsunemimi::Common::Test("FileMethods_Test")
{
    doesPathExist_test();
    doesFileExist_test();
    doesDirExist_test();
    renameFileOrDir_test();
    copyPath_test();
    deleteFileOrDis_test();
}

/**
 * @brief doesPathExist_test
 */
void
FileMethods_Test::doesPathExist_test()
{
    BlossomItem fakeBlossom;
    const std::string testPath = "/tmp/doesPathExist_test_testfile";

    runSyncProcess(fakeBlossom, "rm " + testPath);

    TEST_EQUAL(doesPathExist(testPath), false);

    runSyncProcess(fakeBlossom, "touch " + testPath);

    TEST_EQUAL(doesPathExist(testPath), true);

    runSyncProcess(fakeBlossom, "rm " + testPath);
}

/**
 * @brief doesFileExist_test
 */
void
FileMethods_Test::doesFileExist_test()
{
    BlossomItem fakeBlossom;
    const std::string testFilePath = "/tmp/doesFileExist_test_testfile";
    const std::string testDirPath = "/tmp/doesFileExist_test_testdir";

    runSyncProcess(fakeBlossom, "rm " + testFilePath);
    runSyncProcess(fakeBlossom, "rm " + testDirPath);

    TEST_EQUAL(doesFileExist(testFilePath), false);
    TEST_EQUAL(doesFileExist(testDirPath), false);

    runSyncProcess(fakeBlossom, "touch " + testFilePath);
    runSyncProcess(fakeBlossom, "mkdir " + testDirPath);

    TEST_EQUAL(doesFileExist(testFilePath), true);
    TEST_EQUAL(doesFileExist(testDirPath), false);

    runSyncProcess(fakeBlossom, "rm " + testFilePath);
    runSyncProcess(fakeBlossom, "rm -r " + testDirPath);
}

/**
 * @brief doesDirExist_test
 */
void
FileMethods_Test::doesDirExist_test()
{
    BlossomItem fakeBlossom;
    const std::string testFilePath = "/tmp/doesDirExist_test_testfile";
    const std::string testDirPath = "/tmp/doesDirExist_test_testdir";

    runSyncProcess(fakeBlossom, "rm " + testFilePath);
    runSyncProcess(fakeBlossom, "rm " + testDirPath);

    TEST_EQUAL(doesDirExist(testFilePath), false);
    TEST_EQUAL(doesDirExist(testDirPath), false);

    runSyncProcess(fakeBlossom, "touch " + testFilePath);
    runSyncProcess(fakeBlossom, "mkdir " + testDirPath);

    TEST_EQUAL(doesDirExist(testFilePath), false);
    TEST_EQUAL(doesDirExist(testDirPath), true);

    runSyncProcess(fakeBlossom, "rm " + testFilePath);
    runSyncProcess(fakeBlossom, "rm -r " + testDirPath);
}

/**
 * @brief renameFileOrDir_test
 */
void
FileMethods_Test::renameFileOrDir_test()
{
    BlossomItem fakeBlossom;
    Result result;

    const std::string oldFilePath = "/tmp/renameFileOrDir_test_testfile_ALT";
    const std::string newFileName = "/tmp/renameFileOrDir_test_testfile_NEU";

    runSyncProcess(fakeBlossom, "touch " + oldFilePath);
    runSyncProcess(fakeBlossom, "rm " + newFileName);

    result = renameFileOrDir(oldFilePath, newFileName);
    TEST_EQUAL(result.success, true);

    result = renameFileOrDir(oldFilePath, newFileName);
    TEST_EQUAL(result.success, false);

    TEST_EQUAL(doesPathExist(oldFilePath), false);
    TEST_EQUAL(doesPathExist(newFileName), true);

    runSyncProcess(fakeBlossom, "rm " + oldFilePath);
    runSyncProcess(fakeBlossom, "rm " + newFileName);
}

/**
 * @brief copyPath_test
 */
void
FileMethods_Test::copyPath_test()
{
    BlossomItem fakeBlossom;
    Result result;

    const std::string oldFilePath = "/tmp/copyFile_test_testfile_ALT";
    const std::string newFileName = "/tmp/copyFile_test_testfile_NEU";

    runSyncProcess(fakeBlossom, "touch " + oldFilePath);
    runSyncProcess(fakeBlossom, "rm " + newFileName);

    result = copyPath(oldFilePath, newFileName);
    TEST_EQUAL(result.success, true);

    result = copyPath(oldFilePath, newFileName, false);
    TEST_EQUAL(result.success, false);

    TEST_EQUAL(doesPathExist(oldFilePath), true);
    TEST_EQUAL(doesPathExist(newFileName), true);

    runSyncProcess(fakeBlossom, "rm " + oldFilePath);
    runSyncProcess(fakeBlossom, "rm " + newFileName);
}

/**
 * @brief deleteFileOrDis_test
 */
void
FileMethods_Test::deleteFileOrDis_test()
{
    BlossomItem fakeBlossom;
    const std::string testPath = "/tmp/deleteFileOrDis_test_testfile";

    TEST_EQUAL(deleteFileOrDir(testPath), false);

    runSyncProcess(fakeBlossom, "touch " + testPath);

    TEST_EQUAL(deleteFileOrDir(testPath), true);
    TEST_EQUAL(deleteFileOrDir(testPath), false);

    TEST_EQUAL(doesPathExist(testPath), false);
}

}
