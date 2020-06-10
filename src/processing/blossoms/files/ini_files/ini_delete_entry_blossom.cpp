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

namespace SakuraTree
{

IniDeleteEntryBlossom::IniDeleteEntryBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new DataValue(true));
    m_requiredKeys.insert("group", new DataValue(true));
    m_requiredKeys.insert("entry", new DataValue(false));
}

/**
 * @brief initBlossom
 */
void
IniDeleteEntryBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_group = blossomItem.values.getValueAsString("group");
    m_entry = blossomItem.values.getValueAsString("entry");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
IniDeleteEntryBlossom::preCheck(BlossomItem &blossomItem)
{
    if(Kitsunemimi::Persistence::doesPathExist(m_filePath) == false)
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
IniDeleteEntryBlossom::runTask(BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    std::string fileContent = "";
    bool result = Kitsunemimi::Persistence::readFile(fileContent, m_filePath, errorMessage);

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

    if(m_entry == "") {
        iniItem.removeGroup(m_group);
    } else {
        iniItem.removeEntry(m_group, m_entry);
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
IniDeleteEntryBlossom::postCheck(BlossomItem &blossomItem)
{
    // TODO:
    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
IniDeleteEntryBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
