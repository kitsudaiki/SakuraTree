/**
 * @file        template_create_blossom.h
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

#ifndef TEMPLATE_BLOSSOM_H
#define TEMPLATE_BLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraTree
{
class TemplateCreateBlossom_Test;

class TemplateCreateBlossom
        : public Blossom
{

public:
    TemplateCreateBlossom();

protected:
    void initTask(BlossomItem &blossomItem);
    void preCheck(BlossomItem &blossomItem);
    void runTask(BlossomItem &blossomItem);
    void postCheck(BlossomItem &blossomItem);
    void closeTask(BlossomItem &blossomItem);

private:
    friend TemplateCreateBlossom_Test;

    std::string m_templatePath = "";
    std::string m_destinationPath = "";

    std::string m_fileContent = "";
};

}

#endif // TEMPLATE_BLOSSOM_H
