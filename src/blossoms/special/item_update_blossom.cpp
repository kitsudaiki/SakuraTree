/**
 * @file        item_update_blossom.cpp
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

#include "item_update_blossom.h"

#include <libKitsunemimiSakuraLang/items/value_item_map.h>

/**
 * @brief constructor
 */
ItemUpdateBlossom::ItemUpdateBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
void
ItemUpdateBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::map<std::string, Kitsunemimi::Sakura::ValueItem>::const_iterator overrideIt;
    for(overrideIt = blossomItem.values.m_valueMap.begin();
        overrideIt != blossomItem.values.m_valueMap.end();
        overrideIt++)
    {
        std::map<std::string, DataItem*>::iterator originalIt;
        originalIt = blossomItem.parentValues->m_map.find(overrideIt->first);

        if(originalIt != blossomItem.parentValues->m_map.end()) {
            blossomItem.parentValues->insert(overrideIt->first,
                                             overrideIt->second.item->copy(),
                                             true);
        }
    }
}
