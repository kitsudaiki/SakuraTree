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
#include <processing/blossoms/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TextWriteBlossom::TextWriteBlossom() :
    Blossom() {}

/**
 * @brief initTask
 */
void
TextWriteBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.groupValues.contains("file_path") == false
            || blossomItem.inputValues.contains("text") == false)
    {
        blossomItem.success = false;
        return;
    }

    m_filePath = blossomItem.inputValues.getStringByKey("file_path");
    m_text = blossomItem.inputValues.getStringByKey("text");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
TextWriteBlossom::preCheck(BlossomItem &blossomItem)
{
    if(doesPathExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "TEXT WRITE FAILED: path "
                                   + m_filePath
                                   + " doesn't exist";
        return;
    }

    if(doesFileExist(m_filePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "TEXT WRITE FAILED: path "
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
TextWriteBlossom::runTask(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> result;
    result = Kitsunemimi::Persistence::writeFile(m_filePath, m_text, true);

    if(result.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "TEXT WRITE FAILED: " + result.second;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
TextWriteBlossom::postCheck(BlossomItem &blossomItem)
{

    blossomItem.success = true;
}

/**
 * @brief closeTask
 */
void
TextWriteBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
