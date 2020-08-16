/**
 * @file        path_delete_blossom.cpp
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

#include "path_delete_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>

PathDeleteBlossom::PathDeleteBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new DataValue(true));
}

/**
 * @brief initBlossom
 */
void
PathDeleteBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_path = blossomItem.values.getValueAsString("path");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
PathDeleteBlossom::preCheck(BlossomItem &blossomItem)
{
    if(bfs::exists(m_path) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path doesn't exist: " + m_path;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
PathDeleteBlossom::runTask(BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    const bool result = Kitsunemimi::Persistence::deleteFileOrDir(m_path, errorMessage);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "wasn't able to delete target: " + m_path;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
PathDeleteBlossom::postCheck(BlossomItem &blossomItem)
{
    if(bfs::is_regular_file(m_path))
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path still exist: " + m_path;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
PathDeleteBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
