/**
 * @file        text_append_blossom_test.h
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

#ifndef TEXT_APPEND_BLOSSOM_TEST_H
#define TEXT_APPEND_BLOSSOM_TEST_H

#include <common.h>
#include <libKitsunemimiCommon/test_helper/compare_test_helper.h>

class TextAppendBlossom_Test
        : public Kitsunemimi::CompareTestHelper
{
public:
    TextAppendBlossom_Test();

private:
    void initTestCase();
    void initTask_test();
    void preCheck_test();
    void runTask_test();
    void postCheck_test();
    void closeTask_test();

    std::string m_path = "";
    std::string m_text = "";
    std::string m_newText = "";
};

#endif // TEXT_APPEND_BLOSSOM_TEST_H