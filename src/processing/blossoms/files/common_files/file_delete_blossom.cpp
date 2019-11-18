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

FileDeleteBlossom::FileDeleteBlossom() :
    Blossom() {}

/**
 * @brief initTask
 */
void
FileDeleteBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues->contains("file_path")) {
        blossomItem.success = false;
    }

    m_filePath = blossomItem.inputValues->getStringByKey("file_path");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
FileDeleteBlossom::preCheck(BlossomItem &blossomItem)
{
    const std::pair<bool, std::string> check = doesFileExist(m_filePath);
    if(check.first == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = check.second;
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
    std::string programm = "rm ";

    programm += m_filePath;

    runSyncProcess(blossomItem, programm);
}

/**
 * @brief postCheck
 */
void
FileDeleteBlossom::postCheck(BlossomItem &blossomItem)
{
    const std::pair<bool, std::string> check = doesFileExist(m_filePath);
    if(check.first == true)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = check.second;
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
