/**
 * @file        file_rename_blossom.cpp
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

#include "file_rename_blossom.h"
#include <processing/blossoms/files/file_methods.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

namespace SakuraTree
{
using Kitsunemimi::Common::splitStringByDelimiter;

FileRenameBlossom::FileRenameBlossom() :
    Blossom() {}

/**
 * @brief initTask
 */
void
FileRenameBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues->contains("file_path") == false
            || blossomItem.inputValues->contains("new_name") == false)
    {
        blossomItem.success = false;
    }

    m_filePath = blossomItem.inputValues->getStringByKey("file_path");
    std::vector<std::string> stringParts = splitStringByDelimiter(m_filePath, '/');
    stringParts[stringParts.size()-1] = blossomItem.inputValues->getStringByKey("new_name");

    for(uint32_t i = 0; i < stringParts.size(); i++)
    {
        m_newFilePath += "/";
        m_newFilePath += stringParts.at(i);
    }

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
FileRenameBlossom::preCheck(BlossomItem &blossomItem)
{
    const std::pair<bool, std::string> checkOld = doesFileExist(m_filePath);
    if(checkOld.first == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = checkOld.second;
        return;
    }

    const std::pair<bool, std::string> checkNew = doesPathExist(m_newFilePath);
    if(checkNew.first == true)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "path already exist: " + m_newFilePath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
FileRenameBlossom::runTask(BlossomItem &blossomItem)
{
    std::string programm = "mv ";

    programm += m_filePath;
    programm += " ";
    programm += m_newFilePath;

    runSyncProcess(blossomItem, programm);
}

/**
 * @brief postCheck
 */
void
FileRenameBlossom::postCheck(BlossomItem &blossomItem)
{
    const std::pair<bool, std::string> checkOld = doesFileExist(m_filePath);
    if(checkOld.first == true)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "old file still exist";
        return;
    }

    const std::pair<bool, std::string> checkNew = doesPathExist(m_newFilePath);
    if(checkNew.first == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "was not able to rename to: " + m_newFilePath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief closeTask
 */
void
FileRenameBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
