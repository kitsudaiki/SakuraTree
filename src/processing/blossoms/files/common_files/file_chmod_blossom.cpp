/**
 * @file        file_chmod_blossom.cpp
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

#include "file_chmod_blossom.h"
#include <processing/blossoms/files/file_methods.h>

namespace SakuraTree
{

FileChmodBlossom::FileChmodBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new DataValue(true));
    m_requiredKeys.insert("permission", new DataValue(true));
}

/**
 * @brief initBlossom
 */
void
FileChmodBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_permission = blossomItem.values.getValueAsString("permission");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
FileChmodBlossom::preCheck(BlossomItem &blossomItem)
{
    if(doesPathExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "file-path "
                                   + m_filePath
                                   + " doesn't exist";
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
FileChmodBlossom::runTask(BlossomItem &blossomItem)
{
    std::string command = "chmod ";
    if(doesDirExist(m_filePath)) {
        command += "-R ";
    }
    command += m_permission + " ";
    command += m_filePath;

    runSyncProcess(blossomItem, command);
    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
FileChmodBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
FileChmodBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
