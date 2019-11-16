/**
 * @file        common_methods_test.cpp
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

#include "common_methods_test.h"

#include <sakura_root.h>

namespace SakuraTree
{

/**
 * @brief CommonMethodsTest::CommonMethodsTest
 */
CommonMethodsTest::CommonMethodsTest()
    : Kitsunemimi::Common::Test("CommonMethodsTest")
{
    initTestCase();
    testConvertString();
    testFillItems();
    testOverrideItems();
    testCheckItems();
    cleanupTestCase();
}

/**
 * @brief CommonMethodsTest::initTestCase
 */
void CommonMethodsTest::initTestCase()
{
    m_root = new SakuraRoot(std::string("test"));
}

/**
 * @brief CommonMethodsTest::testConvertString
 */
void CommonMethodsTest::testConvertString()
{
    std::string jinja2String = "{{test}}";
    DataMap obj;
    obj.insert("test", new DataValue("hmmm"));

    std::string result = convertString(jinja2String, obj);
    TEST_EQUAL(result, "hmmm");
}

/**
 * @brief CommonMethodsTest::testFillItems
 */
void CommonMethodsTest::testFillItems()
{
    DataMap items;
    items.insert("x", new DataValue("{{test}}"));
    DataMap insertValues;
    insertValues.insert("test", new DataValue("hmmm"));

    fillItems(items, insertValues);
    TEST_EQUAL(items.get("x")->toString(), "hmmm");
}

/**
 * @brief CommonMethodsTest::testOverrideItems
 */
void CommonMethodsTest::testOverrideItems()
{
    DataMap original;
    original.insert("x", new DataValue("{{test}}"));
    original.insert("y", new DataValue("asdf"));

    JsonItem override;
    override.insert("y", new DataValue("poi"));
    override.insert("z", new DataValue("hmmm"));

    overrideItems(original, override);
    TEST_EQUAL(original.size(), 3);
    TEST_EQUAL(original.get("x")->toString(), "{{test}}");
    TEST_EQUAL(original.get("y")->toString(), "poi");
    TEST_EQUAL(original.get("z")->toString(), "hmmm");
}

/**
 * @brief CommonMethodsTest::testCheckItems
 */
void CommonMethodsTest::testCheckItems()
{
    DataMap items;
    items.insert("x", new DataValue("{{}}"));
    items.insert("y", new DataValue("asdf"));

    std::vector<std::string> result = checkItems(items);
    TEST_EQUAL(result.size(), 1);
    TEST_EQUAL(result.at(0), "x");
}

/**
 * @brief CommonMethodsTest::cleanupTestCase
 */
void CommonMethodsTest::cleanupTestCase()
{
    delete m_root;
}

}
