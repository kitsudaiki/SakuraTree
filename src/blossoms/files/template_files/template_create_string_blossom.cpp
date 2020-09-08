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

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiSakuraLang/sakura_garden.h>

/**
 * @brief constructor
 */
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
void
TemplateCreateStringBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    std::string templatePath = blossomItem.values.getValueAsString("source_path");

    // create source-path
    const bfs::path path = bfs::path("templates") / bfs::path(templatePath);
    templatePath = SakuraRoot::m_interface->garden->getRelativePath(blossomItem.blossomPath,
                                                                        path).string();

    std::string convertedContent = "";
    blossomItem.success = convertTemplate(convertedContent,
                                          templatePath,
                                          blossomItem.values,
                                          blossomItem.outputMessage);

    if(blossomItem.success) {
        blossomItem.blossomOutput.insert("text", new DataValue(convertedContent));
    }
}
