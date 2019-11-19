/**
 * @file        item_methods_test.cpp
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

#include "item_methods_test.h"

#include <sakura_root.h>

namespace SakuraTree
{

/**
 * @brief CommonMethodsTest::CommonMethodsTest
 */
ItemMethods_Test::ItemMethods_Test()
    : Kitsunemimi::Common::Test("ItemMethods_Test")
{
    initTestCase();

    convertString_test();
    fillItems_test();
    overrideItems_test();
    checkItems_test();

    cleanupTestCase();
}

/**
 * @brief initTestCase
 */
void ItemMethods_Test::initTestCase()
{
    m_root = new SakuraRoot(std::string("test"));
}

/**
 * @brief convertString_test
 */
void ItemMethods_Test::convertString_test()
{
    std::string jinja2String = "{{test}}";
    DataMap obj;
    obj.insert("test", new DataValue("hmmm"));

    std::string result = convertString(jinja2String, obj);
    TEST_EQUAL(result, "hmmm");
}

/**
 * @brief fillItems_test
 */
void ItemMethods_Test::fillItems_test()
{
    DataMap items;
    items.insert("x", new DataValue("{{test}}"));
    DataMap insertValues;
    insertValues.insert("test", new DataValue("hmmm"));

    fillItems(items, insertValues);
    TEST_EQUAL(items.get("x")->toString(), "hmmm");
}

/**
 * @brief overrideItems_test
 */
void ItemMethods_Test::overrideItems_test()
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
 * @brief checkItems_test
 */
void ItemMethods_Test::checkItems_test()
{
    DataMap items;
    items.insert("x", new DataValue("{{}}"));
    items.insert("y", new DataValue("asdf"));

    std::vector<std::string> result = checkItems(items);
    TEST_EQUAL(result.size(), 1);
    TEST_EQUAL(result.at(0), "x");
}

/**
 * @brief cleanupTestCase
 */
void ItemMethods_Test::cleanupTestCase()
{
    delete m_root;
}

}
