/**
 *  @file    common_methods_test.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
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

    std::string result = convertString(jinja2String, &obj);
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

    DataMap override;
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

    std::vector<std::string> result = checkItems(&items);
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
