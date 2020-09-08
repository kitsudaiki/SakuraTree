/**
 * @file        template_create_file_blossom.cpp
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

#include "template_create_file_blossom.h"
#include "template_methods.h"

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiSakuraLang/sakura_garden.h>

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

/**
 * @brief constructor
 */
TemplateCreateFileBlossom::TemplateCreateFileBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("dest_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("owner", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("permission", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("variables", new Kitsunemimi::DataValue(true));
}

/**
 * runTask
 */
void
TemplateCreateFileBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string templatePath = blossomItem.values.getValueAsString("source_path");
    const std::string destinationPath = blossomItem.values.getValueAsString("dest_path");
    const std::string owner = blossomItem.values.getValueAsString("owner");
    const std::string permission = blossomItem.values.getValueAsString("permission");

    // create source-path
    const bfs::path path = bfs::path("templates") / bfs::path(templatePath);
    templatePath = SakuraRoot::m_interface->garden->getRelativePath(blossomItem.blossomPath,
                                                                    path).string();


    std::string convertedContent = "";
    blossomItem.success = convertTemplate(convertedContent,
                                          templatePath,
                                          blossomItem.values,
                                          blossomItem.outputMessage);
    if(blossomItem.success == false) {
        return;
    }

    // check if template-file already exist
    if(bfs::exists(destinationPath))
    {
        // read the already existing file and compare it the current file-content
        std::string existingFileContent;
        const bool ret = Kitsunemimi::Persistence::readFile(existingFileContent,
                                                            destinationPath,
                                                            blossomItem.outputMessage);
        if(ret == true
                && convertedContent == existingFileContent)
        {
            blossomItem.skip = true;
            blossomItem.success = true;
            return;
        }
    }



    std::string errorMessage = "";
    blossomItem.success = Kitsunemimi::Persistence::writeFile(destinationPath,
                                                              convertedContent,
                                                              errorMessage,
                                                              true);
    if(blossomItem.success == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "couldn't write template-file to "
                                   + destinationPath +
                                   " with reason: "
                                   + errorMessage;
        return;
    }

    // set owner if defined
    if(owner != "")
    {
        std::string command = "chown ";

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

    // set permission if defined
    if(permission != "")
    {
        std::string command = "chmod ";

        command += permission + " ";
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

    std::string fileContent = "";
    bool ret = Kitsunemimi::Persistence::readFile(fileContent,
                                                  destinationPath,
                                                  blossomItem.outputMessage);
    if(ret == false
            || convertedContent != fileContent)
    {
        blossomItem.success = false;
        return;
    }

    blossomItem.success = true;
}
