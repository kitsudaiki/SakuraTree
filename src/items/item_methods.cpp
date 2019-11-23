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
 * replace the jinja2-converted values of the items-object with the stuff of the insertValues-object
 */
const std::pair<bool, std::string>
fillItems(DataMap &items,
          DataMap &insertValues)
{
    const std::vector<std::string> keys = items.getKeys();
    std::pair<bool, std::string> result;

    for(uint32_t i = 0; i < keys.size(); i++)
    {
        // TODO: make better
        if(keys.at(i) == "subtree")
        {
            items.insert("values", new DataValue(insertValues.toString()));
            continue;
        }

        JsonItem obj = items.get(keys.at(i));
        if(obj.isValue())
        {
            const std::string tempItem = obj.toString();

            Jinja2Converter* converter = SakuraRoot::m_root->m_jinja2Converter;
            const std::pair<bool, std::string> convertResult = converter->convert(tempItem,
                                                                                  &insertValues);

            if(convertResult.first == false)
            {
                result.first = false;
                result.second = "failed to fill value of: " + keys.at(i);
                return result;
            }

            DataValue* value = new DataValue(convertResult.second);
            items.insert(keys.at(i), value, true);
        }
    }

    result.first = true;
    return result;
}

/**
 * @brief overrideItems
 */
void overrideItems(DataMap &original,
                   JsonItem &override)
{
    const std::vector<std::string> keys = override.getKeys();
    for(uint32_t i = 0; i < keys.size(); i++)
    {
        original.insert(keys.at(i),
                        override.get(keys.at(i)).getItemContent()->copy(),
                        true);
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
        DataItem* value = override.get(keys.at(i));
        if(value == nullptr) {
            continue;
        }

        original.insert(keys.at(i),
                        value->copy(),
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
const std::string
convertBlossomOutput(const BlossomItem &blossom)
{
    std::string output = "";
    output += "+++++++++++++++++++++++++++++++++++++++++++++++++\n";

    // print executeion-state
    switch(blossom.resultState)
    {
        case BlossomItem::SKIPPED_STATE:
            output += "SKIPPED\n";
            break;
        case BlossomItem::CHANGED_STATE:
            output += "CHANGED\n";
            break;
        case BlossomItem::ERROR_INIT_STATE:
            output += "ERROR in init-state\n";
            break;
        case BlossomItem::ERROR_PRECHECK_STATE:
            output += "ERROR in pre-check-state\n";
            break;
        case BlossomItem::ERROR_EXEC_STATE:
            output += "ERROR in exec-state with error-code: "
                    + std::to_string(blossom.execState) + "\n";
            break;
        case BlossomItem::ERROR_POSTCHECK_STATE:
            output += "ERROR in post-check-state\n";
            break;
        case BlossomItem::ERROR_CLOSE_STATE:
            output += "ERROR in error-state\n";
            break;
        default:
            break;
    }

    // print call-hierarchy
    for(uint32_t i = 0; i < blossom.nameHirarchie.size(); i++)
    {
        for(uint32_t j = 0; j < i; j++)
        {
            output += "   ";
        }
        output += blossom.nameHirarchie.at(i) + "\n";
    }

    // print error-output
    if(blossom.errorMessage.size() > 0
            && blossom.resultState >= 3)
    {
        // TODO: red error output
        output += "\n";
        output += blossom.errorMessage + "\n";
    }

    // print error-output
    const bool success = blossom.resultState == BlossomItem::CHANGED_STATE
                         || blossom.resultState == BlossomItem::SKIPPED_STATE;
    if(blossom.outputMessage.size() > 0
            && success)
    {
        output += "\n";
        output += blossom.outputMessage + "\n";
    }

    output += "-------------------------------------------------\n";
    output += "\n";

    return output;
}

}
