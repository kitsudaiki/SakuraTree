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

namespace SakuraTree
{

TextReplaceBlossom::TextReplaceBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new DataValue(true));
    m_requiredKeys.insert("old_text", new DataValue(true));
    m_requiredKeys.insert("new_text", new DataValue(true));
}

/**
 * @brief initBlossom
 */
void
TextReplaceBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_oldText = blossomItem.values.getValueAsString("old_text");
    m_newText = blossomItem.values.getValueAsString("new_text");

    blossomItem.success = true;}

/**
 * @brief preCheck
 */
void
TextReplaceBlossom::preCheck(BlossomItem &blossomItem)
{
    if(Kitsunemimi::Persistence::doesPathExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path "
                                   + m_filePath
                                   + " doesn't exist";
        return;
    }

    if(Kitsunemimi::Persistence::doesFileExist(m_filePath) == false)
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
TextReplaceBlossom::runTask(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> result;
    result = Kitsunemimi::Persistence::replaceContent(m_filePath, m_oldText, m_newText);

    if(result.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = result.second;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
TextReplaceBlossom::postCheck(BlossomItem &blossomItem)
{

    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
TextReplaceBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
