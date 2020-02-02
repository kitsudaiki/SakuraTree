/**
 * @file        value_item_map.h
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

#include "value_item_map.h"

namespace SakuraTree
{

/**
 * @brief constructor
 */
ValueItemMap::ValueItemMap() {}

/**
 * @brief copy-constructor
 */
ValueItemMap::ValueItemMap(const ValueItemMap &other)
{
    std::map<std::string, ValueItem>::const_iterator it;
    for(it = other.const_begin();
        it != other.const_end();
        it++)
    {
        ValueItem value = it->second;
        m_valueMap.insert(std::pair<std::string, ValueItem>(it->first, value));
    }
}

/**
 * @brief assignmet-operator
 */
ValueItemMap&
ValueItemMap::operator=(const ValueItemMap &other)
{
    if(this != &other)
    {
        std::map<std::string, ValueItem>::const_iterator it;
        for(it = other.const_begin();
            it != other.const_end();
            it++)
        {
            ValueItem value = it->second;
            this->m_valueMap.insert(std::pair<std::string, ValueItem>(it->first, value));
        }
    }
    return *this;
}

/**
 * @brief add a new key-value-pair to the map
 *
 * @param key key of the new entry
 * @param value data-item of the new entry
 * @param force true, to override, if key already exist.
 *
 * @return true, if new pair was inserted, false, if already exist and force-flag was false
 */
bool
ValueItemMap::insert(const std::string &key,
                     DataItem* value,
                     bool force)
{
    ValueItem valueItem;
    valueItem.item = value->copy();
    return insert(key, valueItem, force);
}

/**
 * @brief add a new key-value-pair to the map
 *
 * @param key key of the new entry
 * @param value value-item of the new entry
 * @param force true, to override, if key already exist.
 *
 * @return true, if new pair was inserted, false, if already exist and force-flag was false
 */
bool
ValueItemMap::insert(const std::string &key,
                     ValueItem &value,
                     bool force)
{
    std::map<std::string, ValueItem>::iterator it;
    it = m_valueMap.find(key);

    if(it != m_valueMap.end()
            && force == false)
    {
        return false;
    }

    if(it != m_valueMap.end()) {
        it->second = value;
    } else {
        m_valueMap.insert(std::pair<std::string, ValueItem>(key, value));
    }
    return true;
}

/**
 * @brief check if the map contains a specific key
 *
 * @param key key to identify the entry
 *
 * @return true, if key exist inside the map, else false
 */
bool
ValueItemMap::contains(const std::string &key)
{
    std::map<std::string, ValueItem>::const_iterator it;
    it = m_valueMap.find(key);

    if(it != m_valueMap.end())
    {
        return true;
    }
    return false;
}

/**
 * @brief remove a value-item from the map
 *
 * @param key key to identify the entry
 *
 * @return true, if item was found and removed, else false
 */
bool
ValueItemMap::remove(const std::string &key)
{
    std::map<std::string, ValueItem>::const_iterator it;
    it = m_valueMap.find(key);

    if(it != m_valueMap.end())
    {
        m_valueMap.erase(it);
        return true;
    }

    return false;
}

/**
 * @brief get data-item inside a value-item of the map as string
 *
 * @param key key to identify the value
 *
 * @return item as string, if found, else empty string
 */
std::string
ValueItemMap::getValueAsString(const std::string &key)
{
    std::map<std::string, ValueItem>::const_iterator it;
    it = m_valueMap.find(key);
    if(it != m_valueMap.end()) {
        return it->second.item->toString();
    }

    return "";
}

/**
 * @brief get data-item inside a value-item of the map
 *
 * @param key key to identify the value
 *
 * @return pointer to the data-item, if found, else a nullptr
 */
DataItem*
ValueItemMap::get(const std::string &key)
{
    std::map<std::string, ValueItem>::const_iterator it;
    it = m_valueMap.find(key);
    if(it != m_valueMap.end()) {
        return it->second.item;
    }

    return nullptr;
}

/**
 * @brief get a value-item from the map
 *
 * @param key key to identify the value
 *
 * @return requested value-item, if found, else an empty uninitialized value-item
 */
ValueItem
ValueItemMap::getValueItem(const std::string &key)
{
    std::map<std::string, ValueItem>::const_iterator it;
    it = m_valueMap.find(key);
    if(it != m_valueMap.end()) {
        return it->second;
    }

    return ValueItem();
}

/**
 * @brief size get number of object in the map
 *
 * @return number of object inside the map
 */
uint64_t
ValueItemMap::size()
{
    return m_valueMap.size();
}

/**
 * @brief iterator begin
 */
std::map<std::string, ValueItem>::iterator
ValueItemMap::begin()
{
    return m_valueMap.begin();
}

/**
 * @brief iterator end
 */
std::map<std::string, ValueItem>::iterator
ValueItemMap::end()
{
    return m_valueMap.end();
}

/**
 * @brief const_iterator begin
 */
std::map<std::string, ValueItem>::const_iterator
ValueItemMap::const_begin() const
{
    return m_valueMap.begin();
}

/**
 * @brief const_iterator end
 */
std::map<std::string, ValueItem>::const_iterator
ValueItemMap::const_end() const
{
    return m_valueMap.end();
}

/**
 * @brief find
 */
std::map<std::string, ValueItem>::iterator
ValueItemMap::find(const std::string key)
{
    return m_valueMap.find(key);
}

}
