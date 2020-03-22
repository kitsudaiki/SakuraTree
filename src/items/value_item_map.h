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

#ifndef VALUE_ITEM_MAP_H
#define VALUE_ITEM_MAP_H

#include <common.h>
#include <items/value_items.h>

namespace SakuraTree
{

class ValueItemMap
{
public:
    ValueItemMap();
    ValueItemMap(const ValueItemMap &other);
    ValueItemMap &operator=(const ValueItemMap &other);

    // add and remove
    bool insert(const std::string &key, DataItem* value, bool force=true);
    bool insert(const std::string &key, ValueItem &value, bool force=true);
    bool remove(const std::string &key);

    // getter
    bool contains(const std::string &key);
    std::string getValueAsString(const std::string &key);
    DataItem* get(const std::string &key);
    ValueItem getValueItem(const std::string &key);
    uint64_t size();
    const std::string toString();

    // iterator
    std::map<std::string, ValueItem>::iterator begin();
    std::map<std::string, ValueItem>::iterator end();
    std::map<std::string, ValueItem>::const_iterator const_begin() const;
    std::map<std::string, ValueItem>::const_iterator const_end() const;
    std::map<std::string, ValueItem>::iterator find(const std::string key);

private:
    std::map<std::string, ValueItem> m_valueMap;
};

}

#endif // VALUE_ITEM_MAP_H
