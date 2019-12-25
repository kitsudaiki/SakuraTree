/**
 * @file        exit_blossom.cpp
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

#include "exit_blossom.h"

namespace SakuraTree
{

ExitBlossom::ExitBlossom()
    : Blossom()
{
    m_requiredKeys.insert("status", new DataValue(false));
}

/**
 * initBlossom
 */
void
ExitBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_exitStatus = blossomItem.values.get("status")->toValue()->getInt();

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
ExitBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
ExitBlossom::runTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;

    exit(m_exitStatus);
}

/**
 * postCheck
 */
void
ExitBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
ExitBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
