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

namespace SakuraTree
{

/**
 * @brief CommonMethodsTest::CommonMethodsTest
 */
CommonMethodsTest::CommonMethodsTest() : Kitsune::Common::Test("CommonMethodsTest")
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
    DataObject obj;
    obj.insert("test", new DataValue("hmmm"));

    std::string result = convertString(jinja2String, &obj);
    UNITTEST(result, "hmmm");
}

/**
 * @brief CommonMethodsTest::testFillItems
 */
void CommonMethodsTest::testFillItems()
{
    DataObject items;
    items.insert("x", new DataValue("{{test}}"));
    DataObject insertValues;
    insertValues.insert("test", new DataValue("hmmm"));

    DataObject* result = fillItems(&items, &insertValues);
    UNITTEST(result->get("x")->toString(), "hmmm");
}

/**
 * @brief CommonMethodsTest::testOverrideItems
 */
void CommonMethodsTest::testOverrideItems()
{
    DataObject original;
    original.insert("x", new DataValue("{{test}}"));
    original.insert("y", new DataValue("asdf"));

    DataObject override;
    override.insert("y", new DataValue("poi"));
    override.insert("z", new DataValue("hmmm"));

    DataObject* result = overrideItems(&original, &override);
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
    DataObject items;
    items.insert("x", new DataValue("{{}}"));
    items.insert("y", new DataValue("asdf"));

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
