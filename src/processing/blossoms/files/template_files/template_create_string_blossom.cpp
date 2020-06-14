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
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <sakura_root.h>
#include <libKitsunemimiJinja2/jinja2_converter.h>
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
    // read template-file
    std::string fileContent = SakuraRoot::m_currentGarden->getTemplate(m_templatePath);
    if(fileContent == "")
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "couldn't find template-file " + m_templatePath;
        return;
    }

    DataMap inputData;
    std::map<std::string, ValueItem>::iterator it;
    for(it = blossomItem.values.begin();
        it != blossomItem.values.end();
        it++)
    {
        if(it->second.item != nullptr) {
            inputData.insert(it->first, it->second.item->copy());
        }
    }

    // create file-content form template
    std::string errorMessage;
    bool jinja2Result = SakuraRoot::m_root->m_jinja2Converter->convert(m_fileContent,
                                                                       fileContent,
                                                                       &inputData,
                                                                       errorMessage);
    if(jinja2Result == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "couldn't convert template-file "
                                   + m_templatePath +
                                   " with reason: "
                                   + errorMessage;
        return;
    }

    // check if template-file already exist
    if(Kitsunemimi::Persistence::doesPathExist(m_destinationPath))
    {
        // read the already existing file and compare it the current file-content
        errorMessage.clear();
        fileContent.clear();
        bool results = Kitsunemimi::Persistence::readFile(fileContent,
                                                          m_destinationPath,
                                                          errorMessage);
        if(results == true
                && m_fileContent == fileContent)
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
                                                           m_fileContent,
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
    bool ret = Kitsunemimi::Persistence::readFile(fileContent, m_destinationPath, errorMessage);
    if(ret == false
            || m_fileContent != fileContent)
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
