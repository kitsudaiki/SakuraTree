/**
 * @file        template_create_blossom.cpp
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

#include "template_create_blossom.h"
#include <processing/blossoms/files/file_methods.h>
#include <sakura_root.h>
#include <libKitsunemimiJinja2/jinja2_converter.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

TemplateCreateBlossom::TemplateCreateBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new DataValue(true));
    m_requiredKeys.insert("dest_path", new DataValue(true));
    m_requiredKeys.insert("*", new DataValue(false));
}

/**
 * initTask
 */
void
TemplateCreateBlossom::initTask(BlossomItem &blossomItem)
{
    m_templatePath = blossomItem.values.getValueAsString("source_path");
    m_destinationPath = blossomItem.values.getValueAsString("dest_path");

    // create source-path
    if(blossomItem.blossomPath.at(blossomItem.blossomPath.size()-1) != '/') {
        blossomItem.blossomPath += "/";
    }
    m_templatePath = blossomItem.blossomPath + "templates/" + m_templatePath;

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
TemplateCreateBlossom::preCheck(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> results;

    // check if path to template is valid
    if(doesPathExist(m_templatePath) == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "CREATE FROM TEMPLATE FAILED: template-path "
                                   + m_templatePath
                                   + " doesn't exist";
        return;
    }

    // read template-file
    results = Kitsunemimi::Persistence::readFile(m_templatePath);
    if(results.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "CREATE FROM TEMPLATE FAILED: couldn't read template-file "
                                   + m_templatePath +
                                   " with reason: "
                                   + results.second;
        return;
    }

    DataMap inputData;
    std::map<std::string, ValueItem>::iterator it;
    for(it = blossomItem.values.valueMap.begin();
        it != blossomItem.values.valueMap.end();
        it++)
    {
        if(it->second.item != nullptr
                && it->second.item->isValue())
        {
            inputData.insert(it->first, it->second.item->copy());
        }
    }

    // create file-content form template
    Kitsunemimi::Jinja2::Jinja2Converter jinja2Converter;
    results = jinja2Converter.convert(results.second, &inputData);
    if(results.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "CREATE FROM TEMPLATE FAILED: couldn't convert template-file "
                                   + m_templatePath +
                                   " with reason: "
                                   + results.second;
        return;
    }
    m_fileContent = results.second;

    // check if template-file already exist
    if(doesPathExist(m_destinationPath))
    {
        // read the already existing file and compare it the current file-content
        results = Kitsunemimi::Persistence::readFile(m_destinationPath);
        if(results.first == true
                && m_fileContent == results.second)
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
TemplateCreateBlossom::runTask(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> results;
    results = Kitsunemimi::Persistence::writeFile(m_destinationPath,
                                                  m_fileContent,
                                                  true);
    if(results.first == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "CREATE FROM TEMPLATE FAILED: couldn't write file "
                                   + m_destinationPath +
                                   " with reason: "
                                   + results.second;
        return;
    }

    blossomItem.success = true;
}

/**
 * postCheck
 */
void
TemplateCreateBlossom::postCheck(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> readFileContent;
    readFileContent = Kitsunemimi::Persistence::readFile(m_destinationPath);
    if(readFileContent.first == false
            || m_fileContent != readFileContent.second)
    {
        blossomItem.success = false;
        return;
    }

    blossomItem.success = true;
}

/**
 * closeTask
 */
void
TemplateCreateBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
