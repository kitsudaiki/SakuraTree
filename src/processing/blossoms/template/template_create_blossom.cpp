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

TemplateCreateBlossom::TemplateCreateBlossom() :
    Blossom() {}

/**
 * initTask
 */
void
TemplateCreateBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues.contains("source_path") == false
            || blossomItem.inputValues.contains("dest_path") == false)
    {
        blossomItem.success = false;
        return;
    }

    m_templatePath = blossomItem.inputValues.getStringByKey("source_path");
    m_templatePath = blossomItem.blossomPath + "/templates/" + m_templatePath;
    m_destinationPath = blossomItem.inputValues.getStringByKey("dest_path");

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
TemplateCreateBlossom::preCheck(BlossomItem &blossomItem)
{
    std::pair<bool, std::string> results;

    if(doesPathExist(m_templatePath) == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "CREATE FROM TEMPLATE FAILED: template-path "
                                   + m_templatePath
                                   + " doesn't exist";
        return;
    }

    results = Kitsunemimi::Persistence::readFile(m_templatePath);
    if(results.first == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "CREATE FROM TEMPLATE FAILED: couldn't read template-file "
                                   + m_templatePath +
                                   " with reason: "
                                   + results.second;
        return;
    }

    results = SakuraRoot::m_jinja2Converter->convert(results.second, &blossomItem.inputValues);
    if(results.first == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "CREATE FROM TEMPLATE FAILED: couldn't convert template-file "
                                   + m_templatePath +
                                   " with reason: "
                                   + results.second;
        return;
    }

    m_fileContent = results.second;

    if(doesPathExist(m_destinationPath))
    {
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
        blossomItem.errorMessage = "CREATE FROM TEMPLATE FAILED: couldn't write file "
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
