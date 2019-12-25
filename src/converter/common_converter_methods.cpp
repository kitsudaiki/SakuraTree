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
 *
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
 * @brief check if all values are set inside a blossom-item, which are required for the
 *        requested blossom-type
 *
 * @param blossomItem blossom-item with the information
 *
 * @return true, if all necessary values are set, else false
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
 * @brief check if all values of a blossom-item match with a list of required keys
 *
 * @param blossomItem blossom-item with the information
 * @param requiredKeys data-map with all required keys. The value behind each key is a
 *                     boolean data-value. If this value is false, the key is optional and don't
 *                     have to be in the values of the blossom-item
 *
 * @return true, if all necessary values are set, else false
 */
bool
checkForRequiredKeys(BlossomItem &blossomItem,
                     DataMap &requiredKeys)
{
    // if "*" is in the list of required key, there is more allowed as the list contains items
    // for example the print-blossom allows all key
    if(requiredKeys.contains("*") == false)
    {
        // check if all keys in the values of the blossom-item also exist in the required-key-list
        std::map<std::string, ValueItem>::const_iterator it;
        for(it = blossomItem.values.const_begin();
            it != blossomItem.values.const_end();
            it++)
        {
            ValueItem tempItem = blossomItem.values.getValueItem(it->first);
            if(tempItem.item == nullptr
                    && tempItem.type == ValueItem::INPUT_PAIR_TYPE)
            {
                // build error-output
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

    // check that all keys in the required keys are also in the values of the blossom-item
    std::map<std::string, DataItem*>::const_iterator it;
    for(it = requiredKeys.m_map.begin();
        it != requiredKeys.m_map.end();
        it++)
    {
        if(it->first == "*") {
            continue;
        }

        // if values of the blossom-item doesn't contain the key and the key is not optional,
        // then create an error-message
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
