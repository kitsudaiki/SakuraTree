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
#include <processing/common/item_methods.h>

namespace SakuraTree
{

ItemUpdateBlossom::ItemUpdateBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new DataValue(false));
}

/**
 * initBlossom
 */
void
ItemUpdateBlossom::initBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
ItemUpdateBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
ItemUpdateBlossom::runTask(BlossomItem &blossomItem)
{
    overrideItems(*blossomItem.parentValues,
                  blossomItem.values,
                  true);
}

/**
 * postCheck
 */
void
ItemUpdateBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
ItemUpdateBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
