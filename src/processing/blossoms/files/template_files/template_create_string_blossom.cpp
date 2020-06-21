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

namespace SakuraTree
{

TemplateCreateStringBlossom::TemplateCreateStringBlossom()
    : Blossom()
{
    m_requiredKeys.insert("source_path", new DataValue(true));
    m_requiredKeys.insert("*", new DataValue(false));
}

/**
 * initBlossom
 */
void
TemplateCreateStringBlossom::initBlossom(BlossomItem &blossomItem)
{
    m_templatePath = blossomItem.values.getValueAsString("source_path");

    // create source-path
    m_templatePath = SakuraRoot::m_currentGarden->getRelativePath(blossomItem.blossomPath,
                                                                  m_templatePath);

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
TemplateCreateStringBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
TemplateCreateStringBlossom::runTask(BlossomItem &blossomItem)
{
    std::string convertedContent = "";
    blossomItem.success = convertTemplate(convertedContent,
                                          m_templatePath,
                                          blossomItem.values,
                                          blossomItem.outputMessage);

    if(blossomItem.success) {
        blossomItem.blossomOutput = new DataValue(convertedContent);
    }
}

/**
 * postCheck
 */
void
TemplateCreateStringBlossom::postCheck(BlossomItem &blossomItem)
{
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
