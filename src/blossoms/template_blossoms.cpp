/**
 * @file        template_blossoms.cpp
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

#include "template_blossoms.h"

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiSakuraLang/sakura_garden.h>
#include <libKitsunemimiSakuraLang/items/sakura_items.h>

#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiPersistence/files/text_file.h>

#include <libKitsunemimiJinja2/jinja2_converter.h>

#include <sakura_root.h>

/**
 * @brief convert a jinja2-template with values into a new string
 *
 * @param output reference for the converted output-string
 * @param templatePath relative-path to the template-file within the sakura-garden
 * @param values value-item-map with all values, which should be inserted into the template
 * @param errorMessage reference-string for output of the error-message in case of an error
 *
 * @return true, if successful, else false
 */
bool
convertTemplate(std::string &output,
                const std::string &templatePath,
                const Kitsunemimi::Sakura::ValueItemMap &values,
                std::string &errorMessage)
{
    // read template-file
    const std::string templateCont = SakuraRoot::m_interface->garden->getTemplate(templatePath);
    if(templateCont == "")
    {
        errorMessage = "couldn't find template-file " + templatePath;
        return false;
    }

    // get variables
    Kitsunemimi::Sakura::ValueItemMap* variables = nullptr;
    std::map<std::string, Kitsunemimi::Sakura::ValueItemMap*>::const_iterator variablesIt;
    variablesIt = values.m_childMaps.find("variables");
    if(variablesIt != values.m_childMaps.end())
    {
        variables = variablesIt->second;
    }
    else
    {
        errorMessage = "couldn't find variables in the template-values";
        return false;
    }

    // convert value-item-map into data-map to be processable by the jinja2-library
    DataMap inputData;
    std::map<std::string, Kitsunemimi::Sakura::ValueItem>::const_iterator it;
    for(it = variables->m_valueMap.begin();
        it != variables->m_valueMap.end();
        it++)
    {
        if(it->second.item != nullptr) {
            inputData.insert(it->first, it->second.item->copy());
        }
    }

    // create file-content form template
    const bool jinja2Result = SakuraRoot::m_interface->jinja2Converter->convert(output,
                                                                                templateCont,
                                                                                &inputData,
                                                                                errorMessage);

    if(jinja2Result == false)
    {
        errorMessage = "couldn't convert template-file "
                       + templatePath +
                       " with reason: "
                       + errorMessage;
        return false;
    }

    return true;
}

//==================================================================================================
// TemplateCreateFileBlossom
//==================================================================================================
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
bool
TemplateCreateFileBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
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
    bool ret = convertTemplate(convertedContent,
                               templatePath,
                               blossomItem.values,
                               errorMessage);
    if(ret == false) {
        return false;
    }

    // check if template-file already exist
    if(bfs::exists(destinationPath))
    {
        // read the already existing file and compare it the current file-content
        std::string existingFileContent;
        const bool ret = Kitsunemimi::Persistence::readFile(existingFileContent,
                                                            destinationPath,
                                                            errorMessage);
        if(ret == true
                && convertedContent == existingFileContent)
        {
            return true;
        }
    }

    ret = Kitsunemimi::Persistence::writeFile(destinationPath,
                                              convertedContent,
                                              errorMessage,
                                              true);
    if(ret == false)
    {
        errorMessage = "couldn't write template-file to "
                       + destinationPath +
                       " with reason: "
                       + errorMessage;
        return false;
    }

    // set owner if defined
    if(owner != "")
    {
        const std::string command = "chown " + owner + ":" + owner + " " + destinationPath;
        if(SakuraRoot::m_root->runCommand(command, errorMessage) == false) {
            return false;
        }
    }

    // set permission if defined
    if(permission != "")
    {
        const std::string command = "chmod " + permission + " " + destinationPath;
        if(SakuraRoot::m_root->runCommand(command, errorMessage) == false) {
            return false;
        }
    }

    std::string fileContent = "";
    ret = Kitsunemimi::Persistence::readFile(fileContent, destinationPath, errorMessage);
    if(ret == false
            || convertedContent != fileContent)
    {
        return false;
    }

    return true;
}

//==================================================================================================
// TemplateCreateStringBlossom
//==================================================================================================
TemplateCreateStringBlossom::TemplateCreateStringBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("source_path", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("variables", new Kitsunemimi::DataValue(true));
}

/**
 * runTask
 */
bool
TemplateCreateStringBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::string templatePath = blossomItem.values.getValueAsString("source_path");

    // create source-path
    const bfs::path path = bfs::path("templates") / bfs::path(templatePath);
    templatePath = SakuraRoot::m_interface->garden->getRelativePath(blossomItem.blossomPath,
                                                                        path).string();

    std::string convertedContent = "";
    const bool ret = convertTemplate(convertedContent,
                                     templatePath,
                                     blossomItem.values,
                                     errorMessage);

    if(ret == false) {
        return false;
    }

    blossomItem.blossomOutput.insert("text", new DataValue(convertedContent));

    return true;
}
