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
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiIni/ini_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

using Kitsunemimi::Ini::IniItem;

IniReadEntryBlossom::IniReadEntryBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("group", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("entry", new Kitsunemimi::DataValue(true));
}

/**
 * @brief initBlossom
 */
void
IniReadEntryBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
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
IniReadEntryBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    if(bfs::exists(m_filePath) == false)
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
IniReadEntryBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
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

    blossomItem.blossomOutput = iniItem.get(m_group, m_entry)->copy();
    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
IniReadEntryBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
IniReadEntryBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
