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

#include <libKitsunemimiSakuraLang/items/item_methods.h>

ItemUpdateBlossom::ItemUpdateBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new Kitsunemimi::DataValue(false));
}

Kitsunemimi::Sakura::Blossom*
ItemUpdateBlossom::createNewInstance()
{
    return new ItemUpdateBlossom();
}

/**
 * initBlossom
 */
void
ItemUpdateBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
ItemUpdateBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
ItemUpdateBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    overrideItems(*blossomItem.parentValues,
                  blossomItem.values,
                  Kitsunemimi::Sakura::ONLY_EXISTING);
}

/**
 * postCheck
 */
void
ItemUpdateBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
ItemUpdateBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
