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

#include <sakura_root.h>

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiSakuraLang/sakura_garden.h>

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

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
 * initBlossom
 */
void
TemplateCreateFileBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    m_templatePath = blossomItem.values.getValueAsString("source_path");
    m_destinationPath = blossomItem.values.getValueAsString("dest_path");
    m_owner = blossomItem.values.getValueAsString("owner");
    m_permission = blossomItem.values.getValueAsString("permission");

    // create source-path
    const bfs::path templatePath = bfs::path("templates") / bfs::path(m_templatePath);
    m_templatePath = SakuraRoot::m_interface->m_garden->getRelativePath(blossomItem.blossomPath,
                                                                        templatePath).string();

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
TemplateCreateFileBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = convertTemplate(m_convertedContent,
                                          m_templatePath,
                                          blossomItem.values,
                                          blossomItem.outputMessage);
    if(blossomItem.success == false) {
        return;
    }

    // check if template-file already exist
    if(bfs::exists(m_destinationPath))
    {
        // read the already existing file and compare it the current file-content
        std::string existingFileContent;
        const bool ret = Kitsunemimi::Persistence::readFile(existingFileContent,
                                                            m_destinationPath,
                                                            blossomItem.outputMessage);
        if(ret == true
                && m_convertedContent == existingFileContent)
        {
            blossomItem.skip = true;
            blossomItem.success = true;
            return;
        }
    }

    blossomItem.success = true;
}

/**
 * runTask
 */
void
TemplateCreateFileBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    blossomItem.success = Kitsunemimi::Persistence::writeFile(m_destinationPath,
                                                              m_convertedContent,
                                                              errorMessage,
                                                              true);
    if(blossomItem.success == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "couldn't write template-file to "
                                   + m_destinationPath +
                                   " with reason: "
                                   + errorMessage;
        return;
    }

    // set owner if defined
    if(m_owner != "")
    {
        std::string command = "chown ";

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

    // set permission if defined
    if(m_permission != "")
    {
        std::string command = "chmod ";

        command += m_permission + " ";
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
 * postCheck
 */
void
TemplateCreateFileBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string fileContent = "";
    bool ret = Kitsunemimi::Persistence::readFile(fileContent,
                                                  m_destinationPath,
                                                  blossomItem.outputMessage);
    if(ret == false
            || m_convertedContent != fileContent)
    {
        blossomItem.success = false;
        return;
    }

    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
TemplateCreateFileBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    blossomItem.success = true;
}
