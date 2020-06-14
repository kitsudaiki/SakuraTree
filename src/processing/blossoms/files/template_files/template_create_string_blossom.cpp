/**
 * @file        template_create_string_blossom.cpp
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

#include "template_create_string_blossom.h"
#include "template_methods.h"

#include <sakura_root.h>

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TemplateCreateStringBlossom::TemplateCreateStringBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new DataValue(true));
    m_requiredKeys.insert("dest_path", new DataValue(true));
    m_requiredKeys.insert("*", new DataValue(false));
}

/**
 * initBlossom
 */
void
TemplateCreateStringBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_templatePath = blossomItem.values.getValueAsString("source_path");
    m_destinationPath = blossomItem.values.getValueAsString("dest_path");

    // create source-path
    m_templatePath = SakuraRoot::m_currentGarden->getRelativePath(blossomItem.blossomPath,
                                                                  m_templatePath,
                                                                  "templates");

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
TemplateCreateStringBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = convertTemplate(m_convertedContent,
                                          m_templatePath,
                                          blossomItem.values,
                                          blossomItem.outputMessage);
    if(blossomItem.success == false) {
        return;
    }

    // check if template-file already exist
    if(Kitsunemimi::Persistence::doesPathExist(m_destinationPath))
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
TemplateCreateStringBlossom::runTask(BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    bool writeResult = Kitsunemimi::Persistence::writeFile(m_destinationPath,
                                                           m_convertedContent,
                                                           errorMessage,
                                                           true);
    if(writeResult == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "couldn't write file "
                                   + m_destinationPath +
                                   " with reason: "
                                   + errorMessage;
        return;
    }

    blossomItem.success = true;
}

/**
 * postCheck
 */
void
TemplateCreateStringBlossom::postCheck(BlossomItem &blossomItem)
{
    std::string errorMessage = "";
    std::string fileContent = "";
    bool ret = Kitsunemimi::Persistence::readFile(fileContent,
                                                  m_destinationPath,
                                                  errorMessage);
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
TemplateCreateStringBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
