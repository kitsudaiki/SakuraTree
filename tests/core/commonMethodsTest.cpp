/**
 *  @file    commonMethodsTest.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "commonMethodsTest.h"

#include <sakuraRoot.h>

namespace SakuraSeed
{

/**
 * @brief CommonMethodsTest::CommonMethodsTest
 */
CommonMethodsTest::CommonMethodsTest() : Kitsune::CommonTest("CommonMethodsTest")
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
    m_root = new SakuraRoot();
}

/**
 * @brief CommonMethodsTest::testConvertString
 */
void CommonMethodsTest::testConvertString()
{
    std::string jinja2String = "{{test}}";
    JsonObject obj;
    obj.insert("test", new JsonValue("hmmm"));

    std::string result = convertString(jinja2String, &obj);
    UNITTEST(result, "hmmm");
}

/**
 * @brief CommonMethodsTest::testFillItems
 */
void CommonMethodsTest::testFillItems()
{
    JsonObject items;
    items.insert("x", new JsonValue("{{test}}"));
    JsonObject insertValues;
    insertValues.insert("test", new JsonValue("hmmm"));

    JsonObject* result = fillItems(&items, &insertValues);
    UNITTEST(result->get("x")->toString(), "hmmm");
}

/**
 * @brief CommonMethodsTest::testOverrideItems
 */
void CommonMethodsTest::testOverrideItems()
{
    JsonObject original;
    original.insert("x", new JsonValue("{{test}}"));
    original.insert("y", new JsonValue("asdf"));

    JsonObject override;
    override.insert("y", new JsonValue("poi"));
    override.insert("z", new JsonValue("hmmm"));

    JsonObject* result = overrideItems(&original, &override);
    UNITTEST(result->getSize(), 3);
    UNITTEST(result->get("x")->toString(), "{{test}}");
    UNITTEST(result->get("y")->toString(), "poi");
    UNITTEST(result->get("z")->toString(), "hmmm");
}

/**
 * @brief CommonMethodsTest::testCheckItems
 */
void CommonMethodsTest::testCheckItems()
{
    JsonObject items;
    items.insert("x", new JsonValue("{{}}"));
    items.insert("y", new JsonValue("asdf"));

    std::vector<std::string> result = checkItems(&items);
    UNITTEST(result.size(), 1);
    UNITTEST(result.at(0), "x");
}

/**
 * @brief CommonMethodsTest::cleanupTestCase
 */
void CommonMethodsTest::cleanupTestCase()
{
    delete m_root;
}

}
