/**
 * @file        ini_blossoms.h
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

#include "ini_blossoms.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiIni/ini_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

using Kitsunemimi::Ini::IniItem;

//==================================================================================================
// IniDeleteEntryBlossom
//==================================================================================================
IniDeleteEntryBlossom::IniDeleteEntryBlossom()
    : Blossom()
{
    validationMap.emplace("file_path", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("group", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("entry", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
}

/**
 * @brief runTask
 */
bool
IniDeleteEntryBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string filePath = blossomLeaf.input.getStringByKey("file_path");
    const std::string group = blossomLeaf.input.getStringByKey("group");
    const std::string entry = blossomLeaf.input.getStringByKey("entry");

    // precheck
    if(bfs::exists(filePath) == false)
    {
        errorMessage = "file-path " + filePath + " doesn't exist";
        return false;
    }

    // read file-content
    std::string fileContent = "";
    const bool result = Kitsunemimi::Persistence::readFile(fileContent, filePath, errorMessage);
    if(result == false) {
        return false;
    }

    // parse file-content
    IniItem iniItem;
    const bool parseSuccess = iniItem.parse(fileContent, errorMessage);
    if(parseSuccess == false) {
        return false;
    }

    // delete entry or entire group
    if(entry == "") {
        iniItem.removeGroup(group);
    } else {
        iniItem.removeEntry(group, entry);
    }

    return true;
}

//==================================================================================================
// IniReadEntryBlossom
//==================================================================================================
IniReadEntryBlossom::IniReadEntryBlossom()
    : Blossom()
{
    validationMap.emplace("file_path", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("group", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("entry", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("value", BlossomValidDef(IO_ValueType::OUTPUT_TYPE, true));
}

/**
 * @brief runTask
 */
bool
IniReadEntryBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string filePath = blossomLeaf.input.getStringByKey("file_path");
    const std::string group = blossomLeaf.input.getStringByKey("group");
    const std::string entry = blossomLeaf.input.getStringByKey("entry");

    // precheck
    if(bfs::exists(filePath) == false)
    {
        errorMessage = "file-path " + filePath + " doesn't exist";
        return false;
    }

    // read file-content
    std::string fileContent = "";
    const bool result = Kitsunemimi::Persistence::readFile(fileContent, filePath, errorMessage);
    if(result == false) {
        return false;
    }

    // parse file-content
    IniItem iniItem;
    const bool parseSuccess = iniItem.parse(fileContent, errorMessage);
    if(parseSuccess == false) {
        return false;
    }

    // write read value into output-values
    blossomLeaf.output.insert("value", iniItem.get(group, entry)->copy());

    return true;
}

//==================================================================================================
// IniSetEntryBlossom
//==================================================================================================
IniSetEntryBlossom::IniSetEntryBlossom()
    : Blossom()
{
    validationMap.emplace("file_path", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("group", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("entry", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("value", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
}

/**
 * @brief runTask
 */
bool
IniSetEntryBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string filePath = blossomLeaf.input.getStringByKey("file_path");
    const std::string group = blossomLeaf.input.getStringByKey("group");
    const std::string entry = blossomLeaf.input.getStringByKey("entry");
    const std::string value = blossomLeaf.input.getStringByKey("value");

    // precheck
    if(bfs::exists(filePath) == false)
    {
        errorMessage = "file-path " + filePath + " doesn't exist";
        return false;
    }

    // read file-content
    std::string fileContent = "";
    const bool result = Kitsunemimi::Persistence::readFile(fileContent, filePath, errorMessage);
    if(result == false) {
        return false;
    }

    // parse file-content
    IniItem iniItem;
    const bool parseResult = iniItem.parse(fileContent, errorMessage);
    if(parseResult == false) {
        return false;
    }

    // set new value
    iniItem.set(group, entry, value, true);

    // write updated string back to file
    const std::string newFileContent = iniItem.toString();
    errorMessage.clear();
    const bool writeResult = Kitsunemimi::Persistence::writeFile(filePath,
                                                                 newFileContent,
                                                                 errorMessage,
                                                                 true);
    if(writeResult == false) {
        return false;
    }

    return true;
}

