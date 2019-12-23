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
#include <processing/common/value_item_functions.h>

using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{

bool
getProcessedItem(ValueItem &original,
                 DataMap &insertValues)
{
    for(uint32_t i = 0; i < original.functions.size(); i++)
    {
        if(original.item == nullptr) {
            return false;
        }

        switch(original.functions.at(i).type)
        {
        case FunctionItem::GET_FUNCTION:
        {
            if(original.functions.at(i).arguments.size() != 1) {
                return false;
            }
            ValueItem arg = original.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues)) {
                original.item = getValue(original.item, arg.item->toValue());
            }
            break;
        }
        case FunctionItem::SPLIT_FUNCTION:
        {
            if(original.functions.at(i).arguments.size() != 1) {
                return false;
            }
            ValueItem arg = original.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues)) {
                original.item = splitValue(original.item->toValue(), arg.item->toValue());
            }
            break;
        }
        case FunctionItem::CONTAINS_FUNCTION:
        {
            if(original.functions.at(i).arguments.size() != 1) {
                return false;
            }
            ValueItem arg = original.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues)) {
                original.item = containsValue(original.item, arg.item->toValue());
            }
            break;
        }
        case FunctionItem::SIZE_FUNCTION:
        {
            original.item = sizeValue(original.item);
            break;
        }
        case FunctionItem::INSERT_FUNCTION:
        {
            if(original.functions.at(i).arguments.size() != 2) {
                return false;
            }
            ValueItem arg1 = original.functions.at(i).arguments.at(0);
            ValueItem arg2 = original.functions.at(i).arguments.at(0);
            if(fillValueItem(arg1, insertValues)
                    && fillValueItem(arg2, insertValues))
            {
                original.item = insertValue(original.item->toMap(),
                                            arg1.item->toValue(),
                                            arg2.item->toValue());
            }
            break;
        }
        case FunctionItem::APPEND_FUNCTION:
        {
            if(original.functions.at(i).arguments.size() != 1) {
                return false;
            }
            ValueItem arg = original.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues)) {
                original.item = appendValue(original.item->toArray(), arg.item->toValue());
            }
            break;
        }
        default:
            break;
        }
    }

    return true;
}

/**
 * @brief fillIdentifierItem
 * @param resulting
 * @param original
 * @param input
 * @return
 */
bool
fillIdentifierItem(ValueItem &original,
                   DataMap &insertValues)
{
    // replace identifier with value from the insert-values
    DataItem* tempItem = insertValues.get(original.item->toString());
    if(tempItem == nullptr) {
        return false;
    }

    delete original.item;
    original.item = tempItem->copy();
    original.functions = original.functions;

    getProcessedItem(original, insertValues);

    return true;
}

/**
 * @brief fillJinja2Template
 * @param baseString
 * @param insertValues
 * @return
 */
bool
fillJinja2Template(ValueItem &original,
                   DataMap &insertValues)
{
    // convert jinja2-string
    Jinja2Converter* converter = SakuraRoot::m_jinja2Converter;
    std::pair<bool, std::string> convertResult;
    convertResult = converter->convert(original.item->toString(), &insertValues);

    ValueItem returnValue;
    if(convertResult.first == false) {
        return false;
    }

    delete original.item;
    original.item = new DataValue(convertResult.second);

    return true;
}

/**
 * @brief fillValueItem
 * @param resulting
 * @param original
 * @param input
 * @return
 */
bool
fillValueItem(ValueItem &original,
              DataMap &insertValues)
{
    if(original.isIdentifier == false
            && original.type != ValueItem::OUTPUT_PAIR_TYPE
            && original.item->isStringValue())
    {
        return fillJinja2Template(original, insertValues);
    }
    else if(original.isIdentifier
            && original.type != ValueItem::OUTPUT_PAIR_TYPE)
    {
        return fillIdentifierItem(original, insertValues);
    }

    return true;
}

/**
 * @brief fillItems
 * @param items
 * @param insertValues
 * @return
 */
bool
fillInputValueItemMap(ValueItemMap &items,
                      DataMap &insertValues)
{
    Result result;

    std::map<std::string, ValueItem>::iterator it;
    for(it = items.begin();
        it != items.end();
        it++)
    {
        if(fillValueItem(it->second, insertValues) == false) {
            return false;
        }
    }

    return true;
}


/**
 * @brief writeOutputBack
 * @param items
 * @param output
 * @return
 */
bool
fillOutputValueItemMap(ValueItemMap &items,
                       DataItem *output)
{
    std::map<std::string, ValueItem>::iterator it;
    for(it = items.begin();
        it != items.end();
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

    return true;
}

/**
 * @brief overrideItems
 * @param original
 * @param override
 * @param onlyExisting
 */
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
void
overrideItems(DataMap &original,
              const ValueItemMap &override,
              bool onlyExisting)
{
    if(onlyExisting)
    {
        std::map<std::string, ValueItem>::const_iterator overrideIt;
        for(overrideIt = override.const_begin();
            overrideIt != override.const_end();
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
        for(overrideIt = override.const_begin();
            overrideIt != override.const_end();
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

    std::map<std::string, ValueItem>::const_iterator it;
    for(it = items.const_begin();
        it != items.const_end();
        it++)
    {
        if(it->second.item->getString() == "{{}}") {
            result.push_back(it->first);
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
