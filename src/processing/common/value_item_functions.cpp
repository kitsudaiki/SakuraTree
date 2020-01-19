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

#include <processing/common/value_item_functions.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

using Kitsunemimi::splitStringByDelimiter;

/**
 * @brief request a value from a map- or array-item
 *
 * @param item map- or array-item
 * @param key key in map or position in array of the requested object
 *
 * @return requested value, if found, else nullptr
 */
DataItem*
getValue(DataItem* item,
         DataValue* key)
{
    // precheck
    if(item == nullptr
            || key == nullptr)
    {
        return nullptr;
    }

    // get value in case of item is a map
    if(item->isMap())
    {
        DataItem* resultItem = item->get(key->toString());
        return resultItem;
    }

    // get value in case of item is an array
    if(item->isArray())
    {
        // check that value for access is an integer
        if(key->isIntValue() == false
                || key->getLong() < 0)
        {
            return nullptr;
        }

        DataItem* resultItem = item->get(static_cast<uint64_t>(key->getLong()));
        return resultItem;
    }

    return nullptr;
}

/**
 * @brief splitValue split a value-item by a delimiter
 *
 * @param item value-item, which should be splited
 * @param delimiter delimiter as string-value to identify the positions, where to split
 *
 * @return array-item with the splitted content
 */
DataArray*
splitValue(DataValue* item,
           DataValue* delimiter)
{
    // precheck
    if(item == nullptr
            || delimiter == nullptr)
    {
        return nullptr;
    }

    // get and check delimiter-string
    const std::string delimiterString = delimiter->toString();
    if(delimiterString.size() == 0) {
        return nullptr;
    }

    // get first character as delimiter and handle line-break as special rule
    char demilimter = delimiterString.at(0);
    if(delimiterString == "\\n") {
        demilimter = '\n';
    }

    // split string into string-array
    std::vector<std::string> array;
    splitStringByDelimiter(array, item->toString(), demilimter);

    // convert string-array into a DataArray-object
    DataArray* resultArray = new DataArray();
    for(uint32_t i = 0; i < array.size(); i++)
    {
        resultArray->append(new DataValue(array.at(i)));
    }

    return resultArray;
}

/**
 * @brief sizeValue get the size of an item
 *
 * @param item data-item, which should be checked
 *
 * @return data-item of int-type with the size of the incoming item as value
 */
DataItem*
sizeValue(DataItem* item)
{
    // precheck
    if(item == nullptr) {
        return nullptr;
    }

    const long size = static_cast<long>(item->size());
    DataValue* resultItem = new DataValue(size);

    return resultItem;
}

/**
 * @brief check if a map or array item contains a specific value
 *
 * @param item data-item, which should be checked
 * @param key value, which should be searched in the item
 *
 * @return data-value with true, if key was found, else data-value with false
 */
DataItem*
containsValue(DataItem* item,
              DataValue* key)
{
    // precheck
    if(item == nullptr
            || key == nullptr)
    {
        return nullptr;
    }

    // in case, that the item is a map
    if(item->isMap())
    {
        const bool result = item->toMap()->contains(key->toString());
        return new DataValue(result);
    }

    // in case, that the item is an array
    if(item->isArray())
    {
        // iterate over the array in interprete all as string for easier comparing
        DataArray* tempArray = item->toArray();
        for(uint32_t i = 0; i < tempArray->size(); i++)
        {
            if(tempArray->get(i)->toString() == key->toString()) {
                return new DataValue(true);
            }
        }

        return new DataValue(false);
    }

    // in case, that the item is a value
    if(item->isValue())
    {
        // interprete this value as string and check if the substring exist in it
        if (item->toString().find(key->toString()) != std::string::npos) {
            return new DataValue(true);
        }

        return new DataValue(false);
    }

    return nullptr;
}

/**
 * @brief add a new object to an existing DataArray-object
 *
 * @param item array-item, which shluld be extended
 * @param value data-item, which should be added
 *
 * @return copy of the original array-item together with the new added object
 */
DataArray*
appendValue(DataArray* item,
            DataItem* value)
{
    // precheck
    if(item == nullptr
            || value == nullptr)
    {
        return nullptr;
    }

    // add oject to the map
    DataArray* result = item->copy()->toArray();
    result->append(value->copy());

    return result;
}

/**
 * @brief delete all empty entries from an array-item
 *
 * @param item array-item, which shluld be cleared
 *
 * @return copy of the original array-item together with the new added object
 */
DataArray*
clearEmpty(DataArray* item)
{
    // precheck
    if(item == nullptr) {
        return nullptr;
    }

    // add oject to the map
    DataArray* result = item->copy()->toArray();
    for(uint32_t i = 0; i < result->size(); i++)
    {
        if(result->get(i)->toString() == "")
        {
            result->remove(i);
            i--;
        }
    }

    return result;
}

/**
 * @brief add a new key-value-pair to an existing DataMap-object
 *
 * @param item pointer to the map-item, where the new pair should be added
 * @param key key of the new pair
 * @param value value of the new pair
 *
 * @return copy of the original map-item together with the new added pair
 */
DataMap*
insertValue(DataMap* item,
            DataValue* key,
            DataItem* value)
{
    // precheck
    if(item == nullptr
            || key == nullptr
            || value == nullptr)
    {
        return nullptr;
    }

    // insert new key-value-pair
    const std::string keyString = key->toString();
    DataMap* result = item->copy()->toMap();
    result->insert(key->toString(), value->copy(), true);

    return result;
}
