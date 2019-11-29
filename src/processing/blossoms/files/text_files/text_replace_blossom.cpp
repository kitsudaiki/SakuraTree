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
#include <processing/blossoms/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TextReplaceBlossom::TextReplaceBlossom()
    : Blossom() {}

/**
 * @brief initTask
 */
void
TextReplaceBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.groupValues.contains("file_path") == false
            || blossomItem.inputValues.contains("old_text") == false
            || blossomItem.inputValues.contains("new_text") == false)
    {
        blossomItem.success = false;
        return;
    }

    m_filePath = blossomItem.groupValues.getStringByKey("file_path");
    m_oldText = blossomItem.inputValues.getStringByKey("old_text");
    m_newText = blossomItem.inputValues.getStringByKey("new_text");

    blossomItem.success = true;}

/**
 * @brief preCheck
 */
void
TextReplaceBlossom::preCheck(BlossomItem &blossomItem)
{
    if(doesPathExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "TEXT REPLACE FAILED: path "
                                   + m_filePath
                                   + " doesn't exist";
        return;
    }

    if(doesFileExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "TEXT REPLACE FAILED: path "
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
        blossomItem.outputMessage = "TEXT REPLACE FAILED: " + result.second;
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
 * @brief closeTask
 */
void
TextReplaceBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
