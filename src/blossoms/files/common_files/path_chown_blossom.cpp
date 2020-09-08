/**
 * @file        path_chown_blossom.cpp
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

#include "path_chown_blossom.h"
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiCommon/process_execution.h>

/**
 * @brief constructor
 */
PathChownBlossom::PathChownBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("owner", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
void
PathChownBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    const std::string path = blossomItem.values.getValueAsString("path");
    const std::string owner = blossomItem.values.getValueAsString("owner");

    // precheck
    if(bfs::exists(path) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path " + path + " doesn't exist";
        return;
    }

    // create command
    std::string command = "chown ";
    if(bfs::is_directory(path)) {
        command += "-R ";
    }
    command += owner + ":" + owner + " ";
    command += path;

    LOG_DEBUG("run command: " + command);

    // run command
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(command);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
}
