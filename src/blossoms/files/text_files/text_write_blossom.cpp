/**
 * @file        text_write_blossom.cpp
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

#include "text_write_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

/**
 * @brief constructor
 */
TextWriteBlossom::TextWriteBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("text", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
void
TextWriteBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string filePath = blossomItem.values.getValueAsString("file_path");
    const std::string text = blossomItem.values.getValueAsString("text");

    std::string errorMessage = "";
    bool result = Kitsunemimi::Persistence::writeFile(filePath, text, errorMessage, true);

    if(result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    blossomItem.success = true;
}
