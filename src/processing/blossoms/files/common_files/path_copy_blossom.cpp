/**
 * @file        path_copy_blossom.cpp
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

#include "path_copy_blossom.h"
#include <sakura_root.h>
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/binary_file.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

namespace SakuraTree
{
using Kitsunemimi::splitStringByDelimiter;

PathCopyBlossom::PathCopyBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new DataValue(true));
    m_requiredKeys.insert("dest_path", new DataValue(true));
    m_requiredKeys.insert("mode", new DataValue(false));
    m_requiredKeys.insert("owner", new DataValue(false));
}

/**
 * @brief initBlossom
 */
void
PathCopyBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_sourcePath = blossomItem.values.getValueAsString("source_path");
    m_destinationPath = blossomItem.values.getValueAsString("dest_path");
    m_mode = blossomItem.values.getValueAsString("mode");
    m_owner = blossomItem.values.getValueAsString("owner");

    if(m_sourcePath.at(0) != '/')
    {
        m_localStorage = true;
        m_sourcePath = SakuraRoot::m_currentGarden->getRelativePath(blossomItem.blossomPath,
                                                                  m_sourcePath,
                                                                  "files");
    }

    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
PathCopyBlossom::preCheck(BlossomItem &blossomItem)
{
    if(m_localStorage == false)
    {
        if(Kitsunemimi::Persistence::doesPathExist(m_sourcePath) == false)
        {
            blossomItem.success = false;
            blossomItem.outputMessage = "COPY FAILED: source-path "
                                       + m_sourcePath
                                       + " doesn't exist";
            return;
        }
    }

    // TODO: compare files, until then copy everytime
    /* if(doesPathExist(m_destinationPath))
    {
        blossomItem.skip = true;
        blossomItem.success = true;
        return;
    }*/

    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
PathCopyBlossom::runTask(BlossomItem &blossomItem)
{
    bool copyResult = false;
    std::string errorMessage = "";

    if(m_localStorage == true)
    {
        Kitsunemimi::DataBuffer* buffer = SakuraRoot::m_currentGarden->getFile(m_sourcePath);

        if(buffer == nullptr)
        {
            blossomItem.success = false;
            blossomItem.outputMessage = "couldn't find local file "
                                       + m_sourcePath;
            return;
        }

        Kitsunemimi::Persistence::deleteFileOrDir(m_destinationPath, errorMessage);
        Kitsunemimi::Persistence::BinaryFile ouputFile(m_destinationPath);
        copyResult = ouputFile.writeCompleteFile(*buffer);
        ouputFile.closeFile();
    }
    else
    {
        copyResult = Kitsunemimi::Persistence::copyPath(m_sourcePath,
                                                        m_destinationPath,
                                                        errorMessage);
    }

    if(copyResult == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    // set mode if requested
    if(m_mode != "")
    {
        std::string command = "chmod ";
        if(Kitsunemimi::Persistence::isDir(m_destinationPath)) {
            command += "-R ";
        }
        command += m_mode + " ";
        command += m_destinationPath;

        LOG_DEBUG("run command: " + command);
        ProcessResult processResult = runSyncProcess(command);
        blossomItem.success = processResult.success;

        if(blossomItem.success == false)
        {
            blossomItem.outputMessage = processResult.processOutput;
            return;
        }
    }

    // set owner if requested
    if(m_owner != "")
    {
        std::string command = "chown ";
        if(Kitsunemimi::Persistence::isDir(m_destinationPath)) {
            command += "-R ";
        }
        command += m_owner + ":" + m_owner + " ";
        command += m_destinationPath;

        LOG_DEBUG("run command: " + command);
        ProcessResult processResult = runSyncProcess(command);
        blossomItem.success = processResult.success;

        if(blossomItem.success == false)
        {
            blossomItem.outputMessage = processResult.processOutput;
            return;
        }
    }

    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
PathCopyBlossom::postCheck(BlossomItem &blossomItem)
{
    if(Kitsunemimi::Persistence::doesPathExist(m_destinationPath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "was not able to copy from "
                                   + m_sourcePath
                                   + " to "
                                   + m_destinationPath;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief closeBlossom
 */
void
PathCopyBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
