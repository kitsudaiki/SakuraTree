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

/**
 * @brief constructor
 */
PathRenameBlossom::PathRenameBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("new_name", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
void
PathRenameBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string path = blossomItem.values.getValueAsString("path");
    std::string newFileName = blossomItem.values.getValueAsString("new_name");

    std::vector<std::string> stringParts;
    splitStringByDelimiter(stringParts, path, '/');
    stringParts[stringParts.size()-1] = newFileName;

    Kitsunemimi::removeEmptyStrings(stringParts);
    for(const std::string& stringPart : stringParts)
    {
         newFileName += "/";
         newFileName += stringPart;
    }


    if(bfs::exists(path) == false
             && bfs::exists(newFileName))
    {
         blossomItem.skip = true;
         blossomItem.success = true;
         return;
    }

    if(bfs::exists(path) == false)
    {
         blossomItem.success = false;
         blossomItem.outputMessage = "source-path "
                                    + path
                                    + " doesn't exist";
         return;
    }


    std::string errorMessage = "";
    bool renameResult = Kitsunemimi::Persistence::renameFileOrDir(path,
                                                                  newFileName,
                                                                  errorMessage);

    if(renameResult == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    if(bfs::is_regular_file(path))
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "old object still exist";
        return;
    }

    if(bfs::exists(newFileName) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "was not able to rename from "
                                   + path
                                   + " to "
                                   + newFileName;
        return;
    }

    blossomItem.success = true;
}

