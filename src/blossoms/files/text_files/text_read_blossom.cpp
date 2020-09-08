/**
 * @file        text_read_blossom.cpp
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

#include "text_read_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

/**
 * @brief constructor
 */
TextReadBlossom::TextReadBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
void
TextReadBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");

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
    std::string fileContent = "";
    bool result = Kitsunemimi::Persistence::readFile(fileContent, filePath, errorMessage);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    blossomItem.blossomOutput.insert("text", new Kitsunemimi::DataValue(fileContent));

    blossomItem.success = true;
}
