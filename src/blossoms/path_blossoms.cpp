/**
 * @file        path_blossoms.cpp
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

#include "path_blossoms.h"

#include <sakura_root.h>

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/binary_file.h>
#include <libKitsunemimiPersistence/logger/logger.h>

#include <libKitsunemimiCommon/process_execution.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/common_methods/vector_methods.h>

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>

using Kitsunemimi::splitStringByDelimiter;


//==================================================================================================
// PathChmodBlossom
//==================================================================================================
PathChmodBlossom::PathChmodBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("permission", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
PathChmodBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string path = blossomLeaf.input.getStringByKey("path");
    const std::string permission = blossomLeaf.input.getStringByKey("permission");

    // precheck
    if(bfs::exists(path) == false)
    {
        errorMessage = "path " + path + " doesn't exist";
        return false;
    }

    // create command
    std::string command = "chmod ";
    if(bfs::is_directory(path)) {
        command += "-R ";
    }
    command += permission + " ";
    command += path;

    return SakuraRoot::m_root->runCommand(command, errorMessage);
}


//==================================================================================================
// PathChownBlossom
//==================================================================================================
PathChownBlossom::PathChownBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("owner", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
PathChownBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string path = blossomLeaf.input.getStringByKey("path");
    const std::string owner = blossomLeaf.input.getStringByKey("owner");

    // precheck
    if(bfs::exists(path) == false)
    {
        errorMessage = "path " + path + " doesn't exist";
        return false;
    }

    // create command
    std::string command = "chown ";
    if(bfs::is_directory(path)) {
        command += "-R ";
    }
    command += owner + ":" + owner + " ";
    command += path;

    return SakuraRoot::m_root->runCommand(command, errorMessage);
}


//==================================================================================================
// PathDeleteBlossom
//==================================================================================================
PathCopyBlossom::PathCopyBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("dest_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("mode", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("owner", new Kitsunemimi::DataValue(false));
}
/**
 * @brief PathCopyBlossom::runTask
 * @param blossomLeaf
 */
bool
PathCopyBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    std::string sourcePath = blossomLeaf.input.getStringByKey("source_path");
    const std::string destinationPath = blossomLeaf.input.getStringByKey("dest_path");
    const std::string mode = blossomLeaf.input.getStringByKey("mode");
    const std::string owner = blossomLeaf.input.getStringByKey("owner");
    bool localStorage = false;

    // prepare source-path
    if(sourcePath.at(0) != '/')
    {
        localStorage = true;
        const bfs::path filePath = bfs::path("files") / bfs::path(sourcePath);
        SakuraLangInterface* interface = SakuraLangInterface::getInstance();
        sourcePath = interface->getRelativePath(blossomLeaf.blossomPath,  filePath).string();
    }

    // precheck
    if(localStorage == false)
    {
        if(bfs::exists(sourcePath) == false)
        {
            errorMessage = "COPY FAILED: source-path " + sourcePath + " doesn't exist";
            return false;
        }
    }

    bool copyResult = false;

    // run task
    if(localStorage == true)
    {
        SakuraLangInterface* interface = SakuraLangInterface::getInstance();
        Kitsunemimi::DataBuffer* buffer = interface->getFile(sourcePath);

        if(buffer == nullptr)
        {
            errorMessage = "couldn't find local file " + sourcePath;
            return false;
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
    if(copyResult == false) {
        return false;
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
        if(processResult.success == false)
        {
            errorMessage = processResult.processOutput;
            return false;
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

        if(SakuraRoot::m_root->runCommand(command, errorMessage) == false) {
            return false;
        }
    }

    // post-check
    if(bfs::exists(destinationPath) == false)
    {
        errorMessage = "was not able to copy from " + sourcePath + " to " + destinationPath;
        return false;
    }

    return true;
}

//==================================================================================================
// PathDeleteBlossom
//==================================================================================================
PathDeleteBlossom::PathDeleteBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
PathDeleteBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string path = blossomLeaf.input.getStringByKey("path");

    // precheck
    if(bfs::exists(path) == false)
    {
        errorMessage = "path doesn't exist: " + path;
        return false;
    }

    const bool result = Kitsunemimi::Persistence::deleteFileOrDir(path, errorMessage);
    if(result == false)
    {
        errorMessage = "wasn't able to delete target: " + path + "\n"
                                    "    error-message: " + errorMessage;
        return false;
    }

    // post-check
    if(bfs::is_regular_file(path))
    {
        errorMessage = "path still exist: " + path;
        return false;
    }

    return true;
}

//==================================================================================================
// PathRenameBlossom
//==================================================================================================
PathRenameBlossom::PathRenameBlossom()
    : Blossom()
{
    m_requiredKeys.insert("path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("new_name", new Kitsunemimi::DataValue(true));
}

/**
 * @brief runTask
 */
bool
PathRenameBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string path = blossomLeaf.input.getStringByKey("path");
    std::string newFileName = blossomLeaf.input.getStringByKey("new_name");

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
         return true;
    }

    if(bfs::exists(path) == false)
    {
         errorMessage = "source-path " + path + " doesn't exist";
         return false;
    }

    const bool ret = Kitsunemimi::Persistence::renameFileOrDir(path, newFileName, errorMessage);
    if(ret == false) {
        return false;
    }

    if(bfs::is_regular_file(path))
    {
        errorMessage = "old object still exist";
        return false;
    }

    if(bfs::exists(newFileName) == false)
    {
        errorMessage = "was not able to rename from " + path + " to " + newFileName;
        return false;
    }

    return true;
}

