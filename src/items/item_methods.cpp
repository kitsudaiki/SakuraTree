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
#include <libKitsunemimiJinja2/jinja2_converter.h>

using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{

/**
 * @brief fillJinja2Template
 * @param baseString
 * @param insertValues
 * @return
 */
const Result
fillJinja2Template(const std::string baseString,
                   const DataMap &insertValues)
{
    Result result;

    // prepare map for jinja2-convert
    DataMap possibleValues;
    std::map<std::string, DataItem*>::const_iterator insertIt;
    for(insertIt = insertValues.m_map.begin();
        insertIt != insertValues.m_map.end();
        insertIt++)
    {
        if(insertIt->second->isValue()) {
            possibleValues.insert(insertIt->first, insertIt->second->copy());
        }
    }

    // convert jinja2-string
    Jinja2Converter* converter = SakuraRoot::m_jinja2Converter;
    const std::pair<bool, std::string> convertResult = converter->convert(baseString,
                                                                          &possibleValues);

    if(convertResult.first == false)
    {
        result.success = false;
        result.errorMessage = convertResult.second;
    }
    else
    {
        result.success = true;
        result.item = new DataValue(convertResult.second);
    }

    return result;
}

/**
 * @brief fillIdentifierItem
 * @param resulting
 * @param original
 * @param input
 * @return
 */
const Result
fillIdentifierItem(ValueItem &resulting,
                   ValueItem &original,
                   DataMap &input)
{
    Result result;

    const std::string key = original.item->toString();

    DataItem* tempItem = input.get(key);
    if(tempItem == nullptr)
    {
        result.success = false;
        result.errorMessage = "couldn't find key: " + key;
        return result;
    }

    resulting.item = tempItem->copy();
    resulting.functions = original.functions;

    for(uint32_t i = 0; i < resulting.functions.size(); i++)
    {
        for(uint32_t j = 0; j < resulting.functions.at(i).arguments.size(); j++)
        {
            const std::string key = resulting.functions[i].arguments[j].toString();
            DataValue replacement = *input.get(key)->toValue();
            resulting.functions[i].arguments[j] = replacement;
        }
    }

    resulting.item = resulting.getProcessedItem();

    result.success = true;

    return result;
}

/**
 * @brief writeOutputBack
 * @param items
 * @param output
 * @return
 */
const Result
fillOutputItems(ValueItemMap &items,
                DataItem *output)
{
    Result result;

    std::map<std::string, ValueItem>::iterator it;
    for(it = items.valueMap.begin();
        it != items.valueMap.end();
        it++)
    {
        if(it->second.type == ValueItem::OUTPUT_PAIR_TYPE)
        {
            ValueItem valueItem;
            valueItem.item = output->copy();
            valueItem.type = ValueItem::OUTPUT_PAIR_TYPE;
            it->second = valueItem;
        }
    }

    result.success = true;

    return result;
}

/**
 * @brief fillItems
 * @param items
 * @param insertValues
 * @return
 */
const Result
fillInputItems(ValueItemMap &items,
               DataMap &insertValues)
{
    Result result;

    std::map<std::string, ValueItem>::iterator it;
    for(it = items.valueMap.begin();
        it != items.valueMap.end();
        it++)
    {
        if(it->second.isIdentifier == false
                && it->second.type == ValueItem::INPUT_PAIR_TYPE
                && it->second.item->isStringValue())
        {
            const Result convertResult = fillJinja2Template(it->second.item->toString(),
                                                            insertValues);

            // process negative result
            if(convertResult.success == false) {
                return convertResult;
            }

            // write positive result back to item-list
            ValueItem valueItem;
            valueItem.item = convertResult.item;
            it->second = valueItem;
        }
        else if(it->second.isIdentifier
                && it->second.type == ValueItem::INPUT_PAIR_TYPE)
        {
            ValueItem valueItem;
            const Result convertResult = fillIdentifierItem(valueItem,
                                                            it->second,
                                                            insertValues);

            // process negative result
            if(convertResult.success == false) {
                return convertResult;
            }

            it->second = valueItem;
        }
    }

    result.success = true;

    return result;
}

void overrideItems(DataMap &original,
                   const DataMap &override,
                   bool onlyExisting)
{
    if(onlyExisting)
    {
        std::map<std::string, DataItem*>::const_iterator overrideIt;
        for(overrideIt = override.m_map.begin();
            overrideIt != override.m_map.end();
            overrideIt++)
        {
            std::map<std::string, DataItem*>::iterator originalIt;
            originalIt = original.m_map.find(overrideIt->first);

            if(originalIt != original.m_map.end()) {
                original.insert(overrideIt->first, overrideIt->second->copy(), true);
            }
        }
    }
    else
    {
        std::map<std::string, DataItem*>::const_iterator overrideIt;
        for(overrideIt = override.m_map.begin();
            overrideIt != override.m_map.end();
            overrideIt++)
        {
            original.insert(overrideIt->first, overrideIt->second->copy(), true);
        }
    }
}

/**
 * @brief overrideItems
 */
void overrideItems(DataMap &original,
                   const ValueItemMap &override,
                   bool onlyExisting)
{
    if(onlyExisting)
    {
        std::map<std::string, ValueItem>::const_iterator overrideIt;
        for(overrideIt = override.valueMap.begin();
            overrideIt != override.valueMap.end();
            overrideIt++)
        {
            std::map<std::string, DataItem*>::iterator originalIt;
            originalIt = original.m_map.find(overrideIt->first);

            if(originalIt != original.m_map.end()) {
                original.insert(overrideIt->first, overrideIt->second.item->copy(), true);
            }
        }
    }
    else
    {
        std::map<std::string, ValueItem>::const_iterator overrideIt;
        for(overrideIt = override.valueMap.begin();
            overrideIt != override.valueMap.end();
            overrideIt++)
        {
            original.insert(overrideIt->first, overrideIt->second.item->copy(), true);
        }
    }
}

/**
 * @brief checkItems
 *
 * @return list of not initialized values
 */
const std::vector<std::string>
checkItems(ValueItemMap &items)
{
    std::vector<std::string> result;

    std::map<std::string, ValueItem>::iterator it;
    for(it = items.valueMap.begin(); it != items.valueMap.end(); it++)
    {
        if(it->second.item->getString() == "{{}}") {
            result.push_back(it->first);
        }
    }

    return result;
}

/**
 * @brief checkForRequiredKeys
 * @param values
 * @param requiredKeys
 * @return
 */
void
checkForRequiredKeys(BlossomItem &blossomItem,
                     const std::vector<std::string> &requiredKeys)
{
    std::string result = "";
    blossomItem.success = true;

    for(uint32_t i = 0; i < requiredKeys.size(); i++)
    {
        const bool found = blossomItem.values.contains(requiredKeys.at(i));
        if(found == false)
        {
            if(result.size() > 0) {
                result += ", ";
            }
            result += requiredKeys.at(i);
        }
    }

    if(result.size() > 0)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "following keys are not set: " + result;
    }
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
    // print call-hierarchy
    for(uint32_t i = 0; i < blossom.nameHirarchie.size(); i++)
    {
        for(uint32_t j = 0; j < i; j++)
        {
            output += "   ";
        }
        output += blossom.nameHirarchie.at(i) + "\n";
    }
    output += "\n";

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

    // print error-output
    if(blossom.outputMessage.size() > 0
            && blossom.resultState >= 3)
    {
        // TODO: red error output
        output += "\n";
        output += blossom.outputMessage + "\n";
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
