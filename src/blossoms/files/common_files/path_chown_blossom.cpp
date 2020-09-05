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

PathChownBlossom::PathChownBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("owner", new Kitsunemimi::DataValue(true));
}

/**
 * @brief initBlossom
 */
void
PathChownBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    m_path = blossomItem.values.getValueAsString("path");
    m_owner = blossomItem.values.getValueAsString("owner");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
PathChownBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    if(bfs::exists(m_path) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "path " + m_path + " doesn't exist";
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
PathChownBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string command = "chown ";

    if(bfs::is_directory(m_path)) {
        command += "-R ";
    }

    command += m_owner + ":" + m_owner + " ";
    command += m_path;

    LOG_DEBUG("run command: " + command);

    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(command);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;
}

/**
 * @brief postCheck
 */
void
PathChownBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
PathChownBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
