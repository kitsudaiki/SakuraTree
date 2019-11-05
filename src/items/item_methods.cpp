/**
 * @file        common_methods.cpp
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

#include "item_methods.h"

#include <libKitsunemimiJinja2/jinja2_converter.h>
#include <sakura_root.h>
#include <processing/blossoms/blossom.h>

using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{

/**
 * @brief convertString
 *
 * @return
 */
const std::string
convertString(const std::string &templateString,
              DataMap *content)
{
    if(content->isMap() == false) {
        return std::string("");
    }

    Jinja2Converter* converter = SakuraRoot::m_root->m_jinja2Converter;
    std::pair<bool, std::string> result = converter->convert(templateString, content);
    // TODO: handle false return value

    return result.second;
}

/**
 * replace the jinja2-converted values of the items-object with the stuff of the insertValues-object
 */
void
fillItems(DataMap &items,
          DataMap &insertValues)
{
    const std::vector<std::string> keys = items.getKeys();

    for(uint32_t i = 0; i < keys.size(); i++)
    {
        DataItem* obj = items.get(keys.at(i));
        if(obj->isValue())
        {
            const std::string tempItem = obj->toString();
            DataValue* value = new DataValue(convertString(tempItem, &insertValues));
            items.insert(keys.at(i), value, true);
        }
    }
}

/**
 * @brief overrideItems
 */
void overrideItems(DataMap &original,
                   DataMap &override)
{
    const std::vector<std::string> keys = override.getKeys();
    for(uint32_t i = 0; i < keys.size(); i++)
    {
        original.insert(keys.at(i),
                        override.get(keys.at(i))->copy(),
                        true);
    }
}

/**
 * @brief checkItems
 *
 * @return list of not initialized values
 */
const std::vector<std::string>
checkItems(DataMap &items)
{
    std::vector<std::string> result;

    const std::vector<std::string> keys = items.getKeys();

    for(uint32_t i = 0; i < keys.size(); i++)
    {
        if(items.get(keys.at(i))->getString() == "{{}}") {
            result.push_back(keys.at(i));
        }
    }

    return result;
}

/**
 * @brief printOutput
 * @param blossom
 */
void
printOutput(const BlossomItem &blossom)
{
    std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

    // print executeion-state
    switch(blossom.resultState)
    {
        case BlossomItem::SKIPPED_STATE:
            std::cout<<"SKIPPED"<<std::endl;
            break;
        case BlossomItem::CHANGED_STATE:
            std::cout<<"CHANGED"<<std::endl;
            break;
        case BlossomItem::ERROR_INIT_STATE:
            std::cout<<"ERROR in init-state"<<std::endl;
            break;
        case BlossomItem::ERROR_PRECHECK_STATE:
            std::cout<<"ERROR in pre-check-state"<<std::endl;
            break;
        case BlossomItem::ERROR_EXEC_STATE:
            std::cout<<"ERROR in exec-state with error-code: "<<blossom.execState<<std::endl;
            break;
        case BlossomItem::ERROR_POSTCHECK_STATE:
            std::cout<<"ERROR in post-check-state"<<std::endl;
            break;
        case BlossomItem::ERROR_CLOSE_STATE:
            std::cout<<"ERROR in error-state"<<std::endl;
            break;
        default:
            break;
    }

    // print call-hirarchie
    for(uint32_t i = 0; i < blossom.nameHirarchie.size(); i++)
    {
        for(uint32_t j = 0; j < i; j++)
        {
            std::cout<<"   ";
        }
        std::cout<<blossom.nameHirarchie.at(i)<<std::endl;
    }

    // print process-output
    if(blossom.outputMessage.size() > 0
            && blossom.resultState >= 3)
    {
        std::cout<<std::endl;
        std::cout<<blossom.outputMessage<<std::endl;
    }

    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<std::endl;
}

}
