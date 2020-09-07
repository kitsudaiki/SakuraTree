/**
 * @file        path_rename_blossom.cpp
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

#include "path_rename_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/common_methods/vector_methods.h>

using Kitsunemimi::splitStringByDelimiter;

PathRenameBlossom::PathRenameBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("new_name", new Kitsunemimi::DataValue(true));
}

Kitsunemimi::Sakura::Blossom*
PathRenameBlossom::createNewInstance()
{
    return new PathRenameBlossom();
}

/**
 * @brief initBlossom
 */
void
PathRenameBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    m_path = blossomItem.values.getValueAsString("path");
    m_newFileName = blossomItem.values.getValueAsString("new_name");

    std::vector<std::string> stringParts;
    splitStringByDelimiter(stringParts, m_path, '/');
    stringParts[stringParts.size()-1] = m_newFileName;

    Kitsunemimi::removeEmptyStrings(stringParts);
    for(const std::string& stringPart : stringParts)
    {
        m_newFilePath += "/";
        m_newFilePath += stringPart;
    }

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
PathRenameBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    if(bfs::exists(m_path) == false
            && bfs::exists(m_newFilePath))
    {
        blossomItem.skip = true;
        blossomItem.success = true;
        return;
    }

    if(bfs::exists(m_path) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "source-path "
                                   + m_path
                                   + " doesn't exist";
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
PathRenameBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    bool renameResult = Kitsunemimi::Persistence::renameFileOrDir(m_path,
                                                                  m_newFilePath,
                                                                  errorMessage);

    if(renameResult == false)
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
PathRenameBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    if(bfs::is_regular_file(m_path))
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "old object still exist";
        return;
    }

    if(bfs::exists(m_newFilePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "was not able to rename from "
                                   + m_path
                                   + " to "
                                   + m_newFileName;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
PathRenameBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
