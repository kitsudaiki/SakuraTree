/**
 * @file        common_converter_methods.cpp
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

#include "common_converter_methods.h"

#include <processing/blossoms/blossom_getter.h>
#include <processing/blossoms/blossom.h>
#include <sakura_root.h>

namespace SakuraTree
{

/**
 * @brief merge two item-maps
 * @param original original item-map
 * @param override additional item-map for merging into the original one
 */
void
overrideItems(JsonItem &original,
              const JsonItem &override)
{
    DataMap* overrideMap = override.getItemContent()->toMap();
    std::map<std::string, DataItem*>::const_iterator it;
    for(it = overrideMap->m_map.begin();
        it != overrideMap->m_map.end();
        it++)
    {
        original.insert(it->first,
                        it->second->copy(),
                        true);
    }
}

/**
 * @brief checkForRequiredKeys
 * @param blossomItem
 * @param blossomGroupType
 * @param blossomType
 * @return
 */
bool
checkForRequiredKeys(BlossomItem &blossomItem)
{
    bool result = false;

    Blossom* blossom = getBlossom(blossomItem.blossomGroupType, blossomItem.blossomType);
    result = checkForRequiredKeys(blossomItem, blossom->m_requiredKeys);
    delete blossom;

    return result;
}

/**
 * @brief checkForRequiredKeys
 * @param values
 * @param requiredKeys
 * @return
 */
bool
checkForRequiredKeys(BlossomItem &blossomItem,
                     DataMap &requiredKeys)
{
    if(requiredKeys.contains("*") == false)
    {
        std::map<std::string, ValueItem>::const_iterator it;
        for(it = blossomItem.values.valueMap.begin();
            it != blossomItem.values.valueMap.end();
            it++)
        {
            ValueItem tempItem = blossomItem.values.getValueItem(it->first);
            if(tempItem.item == nullptr
                    && tempItem.type == ValueItem::INPUT_PAIR_TYPE)
            {
                std::string message = "variable \""
                                      + it->first
                                      + "\" is not in the list of allowed keys";
                SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"ERROR", ""});
                SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"location", "converter"});
                SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"message", message});
                SakuraRoot::m_errorOutput.addRow(
                            std::vector<std::string>{"blossom-type",
                                                     blossomItem.blossomType});
                SakuraRoot::m_errorOutput.addRow(
                            std::vector<std::string>{"blossom-group-type",
                                                     blossomItem.blossomGroupType});
                return false;
            }
        }
    }

    std::map<std::string, DataItem*>::const_iterator it;
    for(it = requiredKeys.m_map.begin();
        it != requiredKeys.m_map.end();
        it++)
    {
        if(it->first == "*") {
            continue;
        }

        if(blossomItem.values.contains(it->first) == false
                && it->second->toValue()->getBool() == true)
        {
            std::string message = "variable \""
                                  + it->first
                                  + "\" is required, but is not set.";
            SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"ERROR", ""});
            SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"location", "converter"});
            SakuraRoot::m_errorOutput.addRow(std::vector<std::string>{"message", message});
            SakuraRoot::m_errorOutput.addRow(
                        std::vector<std::string>{"blossom-type",
                                                 blossomItem.blossomType});
            SakuraRoot::m_errorOutput.addRow(
                        std::vector<std::string>{"blossom-group-type",
                                                 blossomItem.blossomGroupType});
            return false;
        }
    }

    return true;
}

}
