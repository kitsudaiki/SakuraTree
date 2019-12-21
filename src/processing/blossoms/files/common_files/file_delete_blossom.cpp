/**
 * @file        file_delete_blossom.cpp
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

#include "file_delete_blossom.h"
#include <processing/blossoms/files/file_methods.h>

namespace SakuraTree
{

FileDeleteBlossom::FileDeleteBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new DataValue(true));
}

/**
 * @brief initTask
 */
void
FileDeleteBlossom::initTask(BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
FileDeleteBlossom::preCheck(BlossomItem &blossomItem)
{
    if(doesFileExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "file doesn't exist: " + m_filePath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
FileDeleteBlossom::runTask(BlossomItem &blossomItem)
{
    const bool result = deleteFileOrDir(m_filePath);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "wasn't able to delete file: " + m_filePath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
FileDeleteBlossom::postCheck(BlossomItem &blossomItem)
{
    if(doesFileExist(m_filePath))
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "file still exist: " + m_filePath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief closeTask
 */
void
FileDeleteBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
