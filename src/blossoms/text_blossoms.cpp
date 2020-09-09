/**
 * @file        text_blossoms.cpp
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

#include "text_blossoms.h"

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

//==================================================================================================
// TextAppendBlossom
//==================================================================================================
TextAppendBlossom::TextAppendBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("text", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
TextAppendBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string newText = blossomItem.values.getValueAsString("text");

    if(bfs::exists(filePath) == false)
    {
        errorMessage = "path " + filePath + " doesn't exist";
        return false;
    }

    if(bfs::is_regular_file(filePath) == false)
    {
        errorMessage = "path " + filePath + " is not a file";
        return false;
    }

    return Kitsunemimi::Persistence::appendText(filePath, newText, errorMessage);
}

//==================================================================================================
// TextReadBlossom
//==================================================================================================
TextReadBlossom::TextReadBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
TextReadBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");

    if(bfs::exists(filePath) == false)
    {
        errorMessage = "path " + filePath + " doesn't exist";
        return false;
    }

    if(bfs::is_regular_file(filePath) == false)
    {
        errorMessage = "path " + filePath + " is not a file";
        return false;
    }

    std::string fileContent = "";
    const bool result = Kitsunemimi::Persistence::readFile(fileContent, filePath, errorMessage);

    if(result == false) {
        return false;
    }

    blossomItem.blossomOutput.insert("text", new Kitsunemimi::DataValue(fileContent));

    return true;
}

//==================================================================================================
// TextReplaceBlossom
//==================================================================================================
TextReplaceBlossom::TextReplaceBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("old_text", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("new_text", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
TextReplaceBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string oldText = blossomItem.values.getValueAsString("old_text");
    const std::string newText = blossomItem.values.getValueAsString("new_text");

    if(bfs::exists(filePath) == false)
    {
        errorMessage = "path " + filePath + " doesn't exist";
        return false;
    }

    if(bfs::is_regular_file(filePath) == false)
    {
        errorMessage = "path " + filePath + " is not a file";
        return false;
    }

    const bool result = Kitsunemimi::Persistence::replaceContent(filePath,
                                                                 oldText,
                                                                 newText,
                                                                 errorMessage);

    return result;
}

//==================================================================================================
// TextWriteBlossom
//==================================================================================================
TextWriteBlossom::TextWriteBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("text", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
TextWriteBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string text = blossomItem.values.getValueAsString("text");

    const bool result = Kitsunemimi::Persistence::writeFile(filePath,
                                                            text,
                                                            errorMessage,
                                                            true);

    return result;
}
