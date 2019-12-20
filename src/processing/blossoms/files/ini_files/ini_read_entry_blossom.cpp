/**
 * @file        ini_read_entry_blossom.h
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

#include "ini_read_entry_blossom.h"
#include <processing/blossoms/files/file_methods.h>
#include <libKitsunemimiIni/ini_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

using Kitsunemimi::Ini::IniItem;

namespace SakuraTree
{

IniReadEntryBlossom::IniReadEntryBlossom()
    : Blossom() {}

/**
 * @brief initTask
 */
void
IniReadEntryBlossom::initTask(BlossomItem &blossomItem)
{
    const std::vector<std::string> requiredKeys = {"file_path", "group", "entry"};

    checkForRequiredKeys(blossomItem, requiredKeys);
    if(blossomItem.success == false) {
        return;
    }

    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_group = blossomItem.values.getValueAsString("group");
    m_entry = blossomItem.values.getValueAsString("entry");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
IniReadEntryBlossom::preCheck(BlossomItem &blossomItem)
{
    if(doesPathExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "INI-FILE FAILED: file-path "
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
IniReadEntryBlossom::runTask(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> result;
    result = Kitsunemimi::Persistence::readFile(m_filePath);

    if(result.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "INI-FILE FAILED: " + result.second;
        return;
    }

    IniItem iniItem;
    result = iniItem.parse(result.second);

    if(result.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "INI-FILE FAILED: " + result.second;
        return;
    }

    blossomItem.blossomOutput = iniItem.get(m_group, m_entry)->copy();
    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
IniReadEntryBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief closeTask
 */
void
IniReadEntryBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
