/**
 * @file        value_item_functions.cpp
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

#include <items/value_item_functions.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

using Kitsunemimi::Common::splitStringByDelimiter;

/**
 * @brief getValue
 * @param original
 * @param value
 * @return
 */
DataItem* getValue(DataItem* original, DataValue* value)
{
    if(original == nullptr) {
        return nullptr;
    }

    if(original->isMap())
    {
        DataItem* resultItem = original->get(value->toString());
        return resultItem;
    }

    if(original->isArray())
    {
        if(value->isIntValue() == false
                || value->getLong() < 0)
        {
            return nullptr;
        }

        DataItem* resultItem = original->get(static_cast<uint64_t>(value->getLong()));
        return resultItem;
    }

    return nullptr;
}

/**
 * @brief splitValue
 * @param original
 * @param delimiter
 * @return
 */
DataItem* splitValue(DataItem* original, DataValue* delimiter)
{
    if(original == nullptr
            || original->isValue() == false)
    {
        return nullptr;
    }

    const std::string delimiterString = delimiter->toString();
    if(delimiterString.size() == 0) {
        return nullptr;
    }

    const std::vector<std::string> array = splitStringByDelimiter(original->toString(),
                                                                  delimiterString.at(0));

    DataArray* resultArray = new DataArray();
    for(uint32_t i = 0; i < array.size(); i++)
    {
        resultArray->append(new DataValue(array.at(i)));
    }

    return resultArray;
}

/**
 * @brief sizeValue
 * @param original
 * @return
 */
DataItem* sizeValue(DataItem* original)
{
    if(original == nullptr) {
        return nullptr;
    }

    const long size = static_cast<long>(original->size());
    DataValue* resultItem = new DataValue(size);

    return resultItem;
}

/**
 * @brief containsValue
 * @param original
 * @param value
 * @return
 */
DataItem* containsValue(DataItem* original, DataValue* key)
{
    if(original == nullptr) {
        return nullptr;
    }

    if(original->isMap())
    {
        const bool result = original->toMap()->contains(key->toString());
        return new DataValue(result);
    }

    if(original->isArray())
    {
        DataArray* tempArray = original->toArray();
        for(uint32_t i = 0; i < tempArray->size(); i++)
        {
            if(tempArray->get(i)->toString() == key->toString()) {
                return new DataValue(true);
            }
        }

        return new DataValue(false);
    }

    if(original->isValue())
    {
        if (original->toString().find(key->toString()) != std::string::npos) {
            return new DataValue(true);
        }

        return new DataValue(false);
    }

    return nullptr;
}

/**
 * @brief appendValue
 * @param original
 * @param value
 * @return
 */
DataItem* appendValue(DataItem* original, DataValue* value)
{
    if(original == nullptr
            || original->isArray() == false)
    {
        return nullptr;
    }

    original->toArray()->append(value);

    return original;
}

/**
 * @brief insertValue
 * @param original
 * @param key
 * @param value
 * @return
 */
DataItem* insertValue(DataItem* original, DataValue* key, DataValue* value)
{
    if(original == nullptr
            || original->isMap() == false)
    {
        return nullptr;
    }

    const std::string keyString = key->toString();
    original->toMap()->insert(key->toString(), value, true);

    return original;
}
