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

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiSakuraLang/sakura_garden.h>

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/binary_file.h>

#include <libKitsunemimiCommon/common_methods/string_methods.h>

using Kitsunemimi::splitStringByDelimiter;

/**
 * @brief constructor
 */
PathCopyBlossom::PathCopyBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("dest_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("mode", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("owner", new Kitsunemimi::DataValue(false));
}

/**
 * @brief runTask
 */
void
PathCopyBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string sourcePath = blossomItem.values.getValueAsString("source_path");
    const std::string destinationPath = blossomItem.values.getValueAsString("dest_path");
    const std::string mode = blossomItem.values.getValueAsString("mode");
    const std::string owner = blossomItem.values.getValueAsString("owner");
    bool localStorage = false;

    // prepare source-path
    if(sourcePath.at(0) != '/')
    {
        localStorage = true;
        const bfs::path filePath = bfs::path("files") / bfs::path(sourcePath);
        sourcePath = SakuraRoot::m_interface->garden->getRelativePath(blossomItem.blossomPath,
                                                                      filePath).string();
    }

    // precheck
    if(localStorage == false)
    {
        if(bfs::exists(sourcePath) == false)
        {
            blossomItem.success = false;
            blossomItem.outputMessage = "COPY FAILED: source-path "
                                       + sourcePath
                                       + " doesn't exist";
            return;
        }
    }

    bool copyResult = false;
    std::string errorMessage = "";

    // run task
    if(localStorage == true)
    {
        Kitsunemimi::DataBuffer* buffer = SakuraRoot::m_interface->garden->getFile(sourcePath);

        if(buffer == nullptr)
        {
            blossomItem.success = false;
            blossomItem.outputMessage = "couldn't find local file "
                                       + sourcePath;
            return;
        }

        Kitsunemimi::Persistence::deleteFileOrDir(destinationPath, errorMessage);
        Kitsunemimi::Persistence::BinaryFile ouputFile(destinationPath);
        copyResult = ouputFile.writeCompleteFile(*buffer);
        ouputFile.closeFile();
    }
    else
    {
        copyResult = Kitsunemimi::Persistence::copyPath(sourcePath,
                                                        destinationPath,
                                                        errorMessage);
    }

    // check copy-result
    if(copyResult == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = errorMessage;
        return;
    }

    // set mode if requested
    if(mode != "")
    {
        std::string command = "chmod ";
        if(bfs::is_directory(destinationPath)) {
            command += "-R ";
        }
        command += mode + " ";
        command += destinationPath;

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
    if(owner != "")
    {
        std::string command = "chown ";
        if(bfs::is_directory(destinationPath)) {
            command += "-R ";
        }
        command += owner + ":" + owner + " ";
        command += destinationPath;

        LOG_DEBUG("run command: " + command);
        ProcessResult processResult = runSyncProcess(command);
        blossomItem.success = processResult.success;

        if(blossomItem.success == false)
        {
            blossomItem.outputMessage = processResult.processOutput;
            return;
        }
    }

    // post-check
    if(bfs::exists(destinationPath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "was not able to copy from "
                                   + sourcePath
                                   + " to "
                                   + destinationPath;
        return;
    }

    blossomItem.success = true;
}
