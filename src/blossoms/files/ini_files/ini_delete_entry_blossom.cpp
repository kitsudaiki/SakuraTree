/**
 * @file        ini_delete_entry_blossom.h
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

#include "ini_delete_entry_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiIni/ini_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

using Kitsunemimi::Ini::IniItem;

/**
 * @brief constructor
 */
IniDeleteEntryBlossom::IniDeleteEntryBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("group", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("entry", new Kitsunemimi::DataValue(false));
}

/**
 * @brief runTask
 */
void
IniDeleteEntryBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string group = blossomItem.values.getValueAsString("group");
    const std::string entry = blossomItem.values.getValueAsString("entry");

    // precheck
    if(bfs::exists(filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "file-path "
                                   + filePath
                                   + " doesn't exist";
        return;
    }

    std::string errorMessage = "";
    std::string fileContent = "";
    bool result = Kitsunemimi::Persistence::readFile(fileContent, filePath, errorMessage);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    IniItem iniItem;
    errorMessage.clear();
    bool parseSuccess = iniItem.parse(fileContent, errorMessage);

    if(parseSuccess == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    if(entry == "") {
        iniItem.removeGroup(group);
    } else {
        iniItem.removeEntry(group, entry);
    }

    blossomItem.success = true;
}

