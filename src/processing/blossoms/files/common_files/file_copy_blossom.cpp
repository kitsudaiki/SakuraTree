/**
 * @file        file_copy_blossom.cpp
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

#include "file_copy_blossom.h"
#include <processing/blossoms/files/file_methods.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

namespace SakuraTree
{
using Kitsunemimi::Common::splitStringByDelimiter;

FileCopyBlossom::FileCopyBlossom() :
    Blossom() {}

/**
 * @brief initTask
 */
void
FileCopyBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues->contains("source_path") == false
            || blossomItem.inputValues->contains("dest_path") == false)
    {
        blossomItem.success = false;
        return;
    }

    m_sourcePath = blossomItem.inputValues->getStringByKey("source_path");
    m_destinationPath = blossomItem.inputValues->getStringByKey("dest_path");

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
FileCopyBlossom::preCheck(BlossomItem &blossomItem)
{
    if(doesPathExist(m_sourcePath) == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "COPY FAILED: source-path "
                                   + m_sourcePath
                                   + " doesn't exist";
        return;
    }

    if(doesPathExist(m_destinationPath))
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "COPY FAILED: destination-path "
                                   + m_sourcePath
                                   + " already exist";
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
FileCopyBlossom::runTask(BlossomItem &blossomItem)
{
    const std::pair<bool, std::string> copyResult = copyPath(m_sourcePath, m_destinationPath);

    if(copyResult.first == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "COPY FAILED: " + copyResult.second;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
FileCopyBlossom::postCheck(BlossomItem &blossomItem)
{
    if(doesPathExist(m_destinationPath) == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "COPY FAILED: was not able to copy from "
                                   + m_sourcePath
                                   + " to "
                                   + m_destinationPath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief closeTask
 */
void
FileCopyBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
