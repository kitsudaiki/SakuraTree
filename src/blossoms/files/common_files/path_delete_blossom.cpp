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
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
void
PathDeleteBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string path = blossomItem.values.getValueAsString("path");

    // precheck
    if(bfs::exists(path) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path doesn't exist: " + path;
        return;
    }

    std::string errorMessage = "";
    const bool result = Kitsunemimi::Persistence::deleteFileOrDir(path, errorMessage);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "wasn't able to delete target: " + path;
        return;
    }

    // post-check
    if(bfs::is_regular_file(path))
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path still exist: " + path;
        return;
    }

    blossomItem.success = true;
}
