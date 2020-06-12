/**
 * @file        blossom.cpp
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

#include "blossom.h"
#include <sakura_root.h>
#include <processing/common/item_methods.h>

namespace SakuraTree
{

Blossom::Blossom() {}

Blossom::~Blossom() {}

/**
 * @brief SakuraBlossom::growBlossom
 * @return
 */
void
Blossom::growBlossom(BlossomItem &blossomItem)
{
    //----------------------------------------------------------------------------------------------
    LOG_DEBUG("initBlossom " + blossomItem.blossomName);

    initBlossom(blossomItem);

    if(blossomItem.success == false)
    {
        SakuraRoot::m_root->createError(blossomItem,
                                        "blossom init",
                                        blossomItem.outputMessage);
        return;
    }

    //----------------------------------------------------------------------------------------------
    LOG_DEBUG("preCheck " + blossomItem.blossomName);

    preCheck(blossomItem);

    if(blossomItem.success == false)
    {
        SakuraRoot::m_root->createError(blossomItem,
                                        "blossom pre-check",
                                        blossomItem.outputMessage);
        return;
    }

    if(blossomItem.skip) {
        return;
    }

    //----------------------------------------------------------------------------------------------
    LOG_DEBUG("runTask " + blossomItem.blossomName);

    runTask(blossomItem);

    if(blossomItem.success == false)
    {
        SakuraRoot::m_root->createError(blossomItem,
                                        "blossom execute",
                                        blossomItem.outputMessage);
        return;
    }

    //----------------------------------------------------------------------------------------------
    LOG_DEBUG("postCheck " + blossomItem.blossomName);

    postCheck(blossomItem);

    if(blossomItem.success == false)
    {
        SakuraRoot::m_root->createError(blossomItem,
                                        "blossom post-check",
                                        blossomItem.outputMessage);
        return;
    }

    //----------------------------------------------------------------------------------------------
    LOG_DEBUG("closeBlossom " + blossomItem.blossomName);

    closeBlossom(blossomItem);

    if(blossomItem.success == false)
    {
        SakuraRoot::m_root->createError(blossomItem,
                                        "blossom close",
                                        blossomItem.outputMessage);
        return;
    }

    //----------------------------------------------------------------------------------------------

    return;
}

}
