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

TextReplaceBlossom::TextReplaceBlossom()
    : Blossom()
{
    m_requiredKeys.insert("file_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("old_text", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("new_text", new Kitsunemimi::DataValue(true));
}

Kitsunemimi::Sakura::Blossom*
TextReplaceBlossom::createNewInstance()
{
    return new TextReplaceBlossom();
}

/**
 * @brief initBlossom
 */
void
TextReplaceBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    m_filePath = blossomItem.values.getValueAsString("file_path");
    m_oldText = blossomItem.values.getValueAsString("old_text");
    m_newText = blossomItem.values.getValueAsString("new_text");

    blossomItem.success = true;}

/**
 * @brief preCheck
 */
void
TextReplaceBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
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
TextReplaceBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    bool result = Kitsunemimi::Persistence::replaceContent(m_filePath,
                                                           m_oldText,
                                                           m_newText,
                                                           errorMessage);

    if(result == false)
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
TextReplaceBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{

    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
TextReplaceBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
