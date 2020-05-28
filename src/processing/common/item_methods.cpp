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

/**
 * @brief process a value-item by handling its function-calls
 *
 * @param valueItem value-item, which should be processed
 * @param insertValues data-map with information to fill into the jinja2-string
 * @param errorMessage error-message for output
 *
 * @return false, if something went wrong while processing and filling, else true. If false
 *         an error-message was sent directly into the sakura-root-object
*/
bool
getProcessedItem(ValueItem &valueItem,
                 DataMap &insertValues,
                 std::string &errorMessage)
{
    for(uint32_t i = 0; i < valueItem.functions.size(); i++)
    {
        if(valueItem.item == nullptr) {
            return false;
        }

        const std::string type = valueItem.functions.at(i).type;

        //==========================================================================================
        if(type == "get")
        {
            if(valueItem.functions.at(i).arguments.size() != 1) {
                return false;
            }

            ValueItem arg = valueItem.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues, errorMessage))
            {
                valueItem.item = getValue(valueItem.item,
                                          arg.item->toValue(),
                                          errorMessage);
            }
            continue;
        }
        //==========================================================================================
        if(type == "split")
        {
            if(valueItem.functions.at(i).arguments.size() != 1) {
                return false;
            }

            ValueItem arg = valueItem.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues, errorMessage))
            {
                valueItem.item = splitValue(valueItem.item->toValue(),
                                            arg.item->toValue(),
                                            errorMessage);
            }
            continue;
        }
        //==========================================================================================
        if(type == "contains")
        {
            if(valueItem.functions.at(i).arguments.size() != 1) {
                return false;
            }

            ValueItem arg = valueItem.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues, errorMessage))
            {
                valueItem.item = containsValue(valueItem.item,
                                               arg.item->toValue(),
                                               errorMessage);
            }
            continue;
        }
        //==========================================================================================
        if(type == "size")
        {
            valueItem.item = sizeValue(valueItem.item, errorMessage);
            continue;
        }
        //==========================================================================================
        if(type == "insert")
        {
            if(valueItem.functions.at(i).arguments.size() != 2) {
                return false;
            }

            ValueItem arg1 = valueItem.functions.at(i).arguments.at(0);
            ValueItem arg2 = valueItem.functions.at(i).arguments.at(1);

            if(fillValueItem(arg1, insertValues, errorMessage)
                    && fillValueItem(arg2, insertValues, errorMessage))
            {
                valueItem.item = insertValue(valueItem.item->toMap(),
                                             arg1.item->toValue(),
                                             arg2.item,
                                             errorMessage);
            }
            continue;
        }
        //==========================================================================================
        if(type == "append")
        {
            if(valueItem.functions.at(i).arguments.size() != 1) {
                return false;
            }

            ValueItem arg = valueItem.functions.at(i).arguments.at(0);
            if(fillValueItem(arg, insertValues, errorMessage))
            {
                valueItem.item = appendValue(valueItem.item->toArray(),
                                             arg.item,
                                             errorMessage);
            }
            continue;
        }
        //==========================================================================================
        if(type == "clear_empty")
        {
            if(valueItem.functions.at(i).arguments.size() != 0) {
                return false;
            }
            valueItem.item = clearEmpty(valueItem.item->toArray(),
                                        errorMessage);
            continue;
        }
        //==========================================================================================

        valueItem.item = nullptr;
    }

    if(valueItem.item == nullptr) {
        return false;
    }

    return true;
}

/**
 * @brief fill and process an identifier value by filling with incoming information and
 *        processing it functions-calls
 *
 * @param valueItem value-item, which should be processed
 * @param insertValues data-map with information to fill into the jinja2-string
 * @param errorMessage error-message for output
 *
 * @return false, if something went wrong while processing and filling, else true. If false
 *         an error-message was sent directly into the sakura-root-object
 */
bool
fillIdentifierItem(ValueItem &valueItem,
                   DataMap &insertValues,
                   std::string &errorMessage)
{
    // replace identifier with value from the insert-values
    DataItem* tempItem = insertValues.get(valueItem.item->toString());
    if(tempItem == nullptr)
    {
        // TODO: error-message to sakura-root
        return false;
    }

    delete valueItem.item;
    valueItem.item = tempItem->copy();
    valueItem.isIdentifier = false;
    valueItem.functions = valueItem.functions;

    return getProcessedItem(valueItem, insertValues, errorMessage);
}

/**
 * @brief interprete a string as jinja2-string, parse it and fill it with incoming information
 *
 * @param original value-item wiht string-content, which shuold be handled as jinja2-string
 * @param insertValues data-map with information to fill into the jinja2-string
 * @param errorMessage error-message for output
 *
 * @return false, if something went wrong while processing and filling, else true. If false
 *         an error-message was sent directly into the sakura-root-object
 */
bool
fillJinja2Template(ValueItem &valueItem,
                   DataMap &insertValues,
                   std::string &errorMessage)
{
    // convert jinja2-string
    Jinja2Converter* converter = SakuraRoot::m_jinja2Converter;
    std::pair<bool, std::string> convertResult;
    convertResult = converter->convert(valueItem.item->toString(), &insertValues, errorMessage);

    ValueItem returnValue;
    if(convertResult.first == false)
    {
        SakuraRoot::m_root->createError("jinja2-converter", convertResult.second);
        return false;
    }

    delete valueItem.item;
    valueItem.item = new DataValue(convertResult.second);

    return true;
}

/**
 * @brief fill a single value-item with the information of the values of in incoming
 *        data-map, which processing all functions within the value-item-map
 *
 * @param valueItem value-item, which should be processed and filled
 * @param insertValues data-map with the values to fill the value-item
 * @param errorMessage error-message for output
 *
 * @return false, if something went wrong while processing and filling, else true. If false
 *         an error-message was sent directly into the sakura-root-object
 */
bool
fillValueItem(ValueItem &valueItem,
              DataMap &insertValues,
              std::string &errorMessage)
{
    // process and fill incoming string, which is interpreted as jinja2-template
    if(valueItem.isIdentifier == false
            && valueItem.type != ValueItem::OUTPUT_PAIR_TYPE
            && valueItem.item->isStringValue())
    {
        return fillJinja2Template(valueItem, insertValues, errorMessage);
    }
    // process and fill incoming identifier
    else if(valueItem.isIdentifier
            && valueItem.type != ValueItem::OUTPUT_PAIR_TYPE)
    {
        return fillIdentifierItem(valueItem, insertValues, errorMessage);
    }
    else if(valueItem.type != ValueItem::OUTPUT_PAIR_TYPE)
    {
        return getProcessedItem(valueItem, insertValues, errorMessage);
    }

    // if value is an int-value, output-value or something else, then do nothing with the value
    return true;
}

/**
 * @brief fill the entries of a value-item-map with the information of the values of in incoming
 *        data-map, which processing all functions within the value-item-map
 *
 * @param items value-item-map, which should be processed and filled
 * @param insertValues data-map with the values to fill the value-item-map
 * @param errorMessage error-message for output
 *
 * @return false, if something went wrong while processing and filling, else true. If false
 *         an error-message was sent directly into the sakura-root-object
 */
bool
fillInputValueItemMap(ValueItemMap &items,
                      DataMap &insertValues,
                      std::string &errorMessage)
{
    Result result;

    std::map<std::string, ValueItem>::iterator it;
    for(it = items.begin();
        it != items.end();
        it++)
    {
        if(fillValueItem(it->second, insertValues, errorMessage) == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief wirte the output of a blossom-item back into a value-item-map
 *
 * @param items value-item-map, where the output should be inserted
 * @param output output of the blossom-item as data-item
 *
 * @return true, if the output was written in at least one point of the value-item-map
 */
bool
fillOutputValueItemMap(ValueItemMap &items,
                       DataItem* output)
{
    bool found = false;

    std::map<std::string, ValueItem>::iterator it;
    for(it = items.begin();
        it != items.end();
        it++)
    {
        // replace only as output-marked values
        if(it->second.type == ValueItem::OUTPUT_PAIR_TYPE)
        {
            ValueItem valueItem;
            valueItem.item = output->copy();
            valueItem.type = ValueItem::OUTPUT_PAIR_TYPE;
            it->second = valueItem;
            found = true;
        }
    }

    return found;
}

/**
 * @brief override data of a data-map with new incoming information
 *
 * @param original data-map with the original key-values, which should be updates with the
 *                 information of the override-map
 * @param override map with the new incoming information
 * @param onlyExisting true, of only replacing values of existing key, but not add new keys
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
 * @brief override data of a data-map with new incoming information
 *
 * @param original data-map with the original key-values, which should be updates with the
 *                 information of the override-map
 * @param override map with the new incoming information
 * @param onlyExisting true, of only replacing values of existing key, but not add new keys
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
 * @brief override data of a value-item-map with new incoming information
 *
 * @param original map with the original key-values, which should be updates with the
 *                 information of the override-map
 * @param override map with the new incoming information
 * @param onlyExisting true, of only replacing values of existing key, but not add new keys
 */
void
overrideItems(ValueItemMap &original,
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
            std::map<std::string, ValueItem>::iterator originalIt;
            originalIt = original.find(overrideIt->first);

            if(originalIt != original.end())
            {
                ValueItem temp = overrideIt->second;
                original.insert(overrideIt->first, temp, true);
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
            ValueItem temp = overrideIt->second;
            original.insert(overrideIt->first, temp, true);
        }
    }
}

/**
 * @brief check value-item-map for uninitialized values
 *
 * @param items value-map to check
 *
 * @return list of not initialized values
 */
const std::vector<std::string>
checkItems(DataMap &items)
{
    std::vector<std::string> result;

    std::map<std::string, DataItem*>::const_iterator it;
    for(it = items.m_map.begin();
        it != items.m_map.end();
        it++)
    {
        if(it->second->getString() == "{{}}") {
            result.push_back(it->first);
        }
    }

    return result;
}

/**
 * @brief print output of a blossom-item
 *
 * @param blossom item with the information to print
 *
 * @return output as string
 */
const std::string
convertBlossomOutput(const BlossomItem &blossom)
{
    // get width of the termial to draw the separator-line
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    const uint32_t terminalWidth = size.ws_col;

    // draw separator line
    std::string output(terminalWidth, '=');
    output += "\n\n";

    // print call-hierarchy
    for(uint32_t i = 0; i < blossom.nameHirarchie.size(); i++)
    {
        for(uint32_t j = 0; j < i; j++)
        {
            output += "   ";
        }
        output += blossom.nameHirarchie.at(i) + "\n";
    }

    // print executeion-state
    if(blossom.skip) {
        output += "SKIPPED\n";
    }

    // print error-output
    if(blossom.success == false)
    {
        // TODO: red error output
        output += "\n";
        output += SakuraRoot::m_root->m_errorOutput.toString(200) + "\n";
    }

    // print error-output
    if(blossom.outputMessage.size() > 0
            && blossom.success)
    {
        output += "\n";
        output += blossom.outputMessage + "\n";
    }

    return output;
}

}
