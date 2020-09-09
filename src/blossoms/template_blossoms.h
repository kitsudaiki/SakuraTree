/**
 * @file        template_blossoms.h
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

#ifndef TEMPLATE_BLOSSOMS_H
#define TEMPLATE_BLOSSOMS_H

#include <common.h>

//==================================================================================================
// TemplateCreateFileBlossom
//==================================================================================================
class TemplateCreateFileBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    TemplateCreateFileBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// TemplateCreateStringBlossom
//==================================================================================================
class TemplateCreateStringBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    TemplateCreateStringBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

#endif // TEMPLATE_BLOSSOMS_H
