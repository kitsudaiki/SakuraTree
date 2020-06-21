/**
 * @file        ini_set_entry_blossom.h
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

#include "ini_set_entry_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiIni/ini_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

using Kitsunemimi::Ini::IniItem;

namespace SakuraTree
{

IniSetEntryBlossom::IniSetEntryBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new DataValue(true));
    m_requiredKeys.insert("group", new DataValue(true));
    m_requiredKeys.insert("entry", new DataValue(true));
    m_requiredKeys.insert("value", new DataValue(true));
}

/**
 * @brief initBlossom
 */
void
IniSetEntryBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_group = blossomItem.values.getValueAsString("group");
    m_entry = blossomItem.values.getValueAsString("entry");
    m_value = blossomItem.values.getValueAsString("value");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
IniSetEntryBlossom::preCheck(BlossomItem &blossomItem)
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
IniSetEntryBlossom::runTask(BlossomItem &blossomItem)
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
    bool parseResult = iniItem.parse(fileContent, errorMessage);

    if(parseResult == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    iniItem.set(m_group, m_entry, m_value, true);

    const std::string newFileContent = iniItem.toString();
    errorMessage.clear();
    bool writeResult = Kitsunemimi::Persistence::writeFile(m_filePath,
                                                           newFileContent,
                                                           errorMessage,
                                                           true);
    if(writeResult == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
IniSetEntryBlossom::postCheck(BlossomItem &blossomItem)
{
    // TODO
    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
IniSetEntryBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
