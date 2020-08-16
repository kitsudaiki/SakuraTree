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

TextReadBlossom::TextReadBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("file_path", new DataValue(true));
}

/**
 * @brief initBlossom
 */
void
TextReadBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");
    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
TextReadBlossom::preCheck(BlossomItem &blossomItem)
{
    if(bfs::exists(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path "
                                   + m_filePath
                                   + " doesn't exist";
        return;
    }

    if(bfs::is_regular_file(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path "
                                   + m_filePath
                                   + " is not a file";
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
TextReadBlossom::runTask(BlossomItem &blossomItem)
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

    blossomItem.blossomOutput = new DataValue(fileContent);
    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
TextReadBlossom::postCheck(BlossomItem &blossomItem)
{

    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
TextReadBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
