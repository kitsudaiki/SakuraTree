/**
 * @file        register_node_blossom.cpp
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

#include "register_node_blossom.h"
#include <sakura_root.h>
#include <libKitsunemimiSakuraNetwork/sakura_network.h>

namespace SakuraTree
{

RegisterNodeBlossom::RegisterNodeBlossom()
    : Blossom()
{
    m_requiredKeys.insert("host_name", new DataValue(true));
}

/**
 * initBlossom
 */
void
RegisterNodeBlossom::initBlossom(BlossomItem &blossomItem)
{
    hostName = blossomItem.parentValues->get("host_name")->toString();

    DataArray* array = blossomItem.parentValues->get("type")->toArray();
    for(uint32_t i = 0; i < array->size(); i++)
    {
        tags.push_back(array->m_array.at(i)->toString());
    }

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
RegisterNodeBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
RegisterNodeBlossom::runTask(BlossomItem &blossomItem)
{
    const bool result = SakuraRoot::m_root->m_networking->registerHost(hostName, tags);
    blossomItem.success = result;
}

/**
 * postCheck
 */
void
RegisterNodeBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
RegisterNodeBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
