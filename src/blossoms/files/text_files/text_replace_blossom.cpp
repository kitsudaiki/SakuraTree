/**
 * @file        text_replace_blossom.cpp
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

#include "text_replace_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

/**
 * @brief constructor
 */
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
void
TextReplaceBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string oldText = blossomItem.values.getValueAsString("old_text");
    const std::string newText = blossomItem.values.getValueAsString("new_text");

    if(bfs::exists(filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path "
                                   + filePath
                                   + " doesn't exist";
        return;
    }

    if(bfs::is_regular_file(filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path "
                                   + filePath
                                   + " is not a file";
        return;
    }


    std::string errorMessage = "";
    bool result = Kitsunemimi::Persistence::replaceContent(filePath,
                                                           oldText,
                                                           newText,
                                                           errorMessage);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    blossomItem.success = true;
}
