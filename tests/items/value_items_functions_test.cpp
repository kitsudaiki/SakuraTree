/**
 * @file        value_items_functions_test.cpp
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

#include "value_items_functions_test.h"

namespace SakuraTree
{

/**
 * @brief constructor
 */
ValueItemsFunctions_Test::ValueItemsFunctions_Test()
    : Kitsunemimi::Common::Test("ValueItemsFunctions_Test")
{
    getValue_test();
    splitValue_test();
    sizeValue_test();
    containsValue_test();
    appendValue_test();
    insertValue_test();
}

void
ValueItemsFunctions_Test::checkNullptr(DataItem* value)
{
    const bool isNullptr = value == nullptr;
    TEST_EQUAL(isNullptr, true);
}

/**
 * @brief getValue_test
 */
void
ValueItemsFunctions_Test::getValue_test()
{
    DataArray testArray;
    testArray.append(new DataValue("poi"));
    testArray.append(new DataValue(42));

    DataItem* result = getValue(&testArray, new DataValue(1));

    TEST_EQUAL(result->isIntValue(), true);
    TEST_EQUAL(result->toString(), "42");

    DataMap testMap;
    testMap.insert("asdf", new DataValue("poi"));
    testMap.insert("xyz", new DataValue(42));

    result = getValue(&testMap, new DataValue("asdf"));

    TEST_EQUAL(result->isStringValue(), true);
    TEST_EQUAL(result->toString(), "poi");


    // negative tests
    result = getValue(nullptr, new DataValue("asdf"));
    checkNullptr(result);

    result = getValue(&testArray, new DataValue("asdf"));
    checkNullptr(result);

    result = getValue(&testMap, new DataValue(1));
    checkNullptr(result);
}

/**
 * @brief splitValue_test
 */
void
ValueItemsFunctions_Test::splitValue_test()
{
    DataValue testValue("this is a test string");

    DataItem* result = splitValue(&testValue, new DataValue(" "));

    TEST_EQUAL(result->isArray(), true);
    TEST_EQUAL(result->toArray()->size(), 5);
    TEST_EQUAL(result->toArray()->get(3)->toString(), "test");


    // negative tests
    result = splitValue(nullptr, new DataValue(""));
    checkNullptr(result);

    result = splitValue(&testValue, new DataValue(""));
    checkNullptr(result);
}

/**
 * @brief sizeValue_test
 */
void
ValueItemsFunctions_Test::sizeValue_test()
{
    DataArray testArray;
    testArray.append(new DataValue("poi"));
    testArray.append(new DataValue(42));

    DataItem* result = sizeValue(&testArray);

    TEST_EQUAL(result->isIntValue(), true);
    TEST_EQUAL(result->toValue()->getLong(), 2);

    DataMap testMap;
    testMap.insert("asdf", new DataValue("poi"));
    testMap.insert("xyz", new DataValue(42));

    result = sizeValue(&testMap);

    TEST_EQUAL(result->isIntValue(), true);
    TEST_EQUAL(result->toValue()->getLong(), 2);


    // negative tests
    result = sizeValue(nullptr);
    checkNullptr(result);
}

/**
 * @brief containsValue_test
 */
void
ValueItemsFunctions_Test::containsValue_test()
{
    DataItem* result = nullptr;

    // map-type
    DataMap testMap;
    testMap.insert("asdf", new DataValue("poi"));
    testMap.insert("xyz", new DataValue(42));

    result = containsValue(&testMap, new DataValue("asdf"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), true);

    result = containsValue(&testMap, new DataValue("12345"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), false);


    // array-type
    DataArray testArray;
    testArray.append(new DataValue("poi"));
    testArray.append(new DataValue(42));

    result = containsValue(&testArray, new DataValue("poi"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), true);

    result = containsValue(&testArray, new DataValue("42"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), true);

    result = containsValue(&testArray, new DataValue("12345"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), false);


    // value-type
    DataValue testValue("this is a test-string");

    result = containsValue(&testValue, new DataValue("this"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), true);

    result = containsValue(&testValue, new DataValue("is a test-s"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), true);

    result = containsValue(&testValue, new DataValue("12345"));

    TEST_EQUAL(result->isBoolValue(), true);
    TEST_EQUAL(result->toValue()->getBool(), false);
}

/**
 * @brief appendValue_test
 */
void
ValueItemsFunctions_Test::appendValue_test()
{
    DataArray testArray;
    testArray.append(new DataValue("poi"));
    testArray.append(new DataValue(42));

    DataItem* result = sizeValue(&testArray);
    TEST_EQUAL(result->toValue()->getLong(), 2);

    result = appendValue(&testArray, new DataValue("12345"));

    result = sizeValue(&testArray);
    TEST_EQUAL(result->toValue()->getLong(), 3);

    // negative tests
    result = appendValue(new DataMap(), new DataValue("12345"));
    checkNullptr(result);

    result = appendValue(nullptr, new DataValue("12345"));
    checkNullptr(result);
}

/**
 * @brief insertValue_test
 */
void
ValueItemsFunctions_Test::insertValue_test()
{
    DataMap testMap;
    testMap.insert("asdf", new DataValue("poi"));
    testMap.insert("xyz", new DataValue(42));

    DataItem* result = containsValue(&testMap, new DataValue("12345"));
    TEST_EQUAL(result->toValue()->getBool(), false);

    result = insertValue(&testMap, new DataValue("12345"), new DataValue("12345"));

    result = containsValue(&testMap, new DataValue("12345"));
    TEST_EQUAL(result->toValue()->getBool(), true);


    // negative tests
    result = insertValue(new DataArray(), new DataValue("12345"), new DataValue("12345"));
    checkNullptr(result);

    result = insertValue(nullptr, new DataValue("12345"), new DataValue("12345"));
    checkNullptr(result);
}

}
