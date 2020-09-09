/**
 * @file        special_blossoms.cpp
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

#include "special_blossoms.h"

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/process_execution.h>
#include <libKitsunemimiCommon/common_items/table_item.h>

//==================================================================================================
// PrintBlossom
//==================================================================================================
AssertBlossom::AssertBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
bool
AssertBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::map<std::string, Kitsunemimi::Sakura::ValueItem>::iterator it;
    for(it = blossomItem.values.m_valueMap.begin();
        it != blossomItem.values.m_valueMap.end();
        it++)
    {
        const std::string isValue = blossomItem.parentValues->get(it->first)->toString();
        const std::string shouldValue = it->second.item->toString();

        if(isValue != shouldValue)
        {
            errorMessage = "the variable \""
                           + it->first
                           + "\" has the value \""
                           + isValue
                           + "\", but it should have the value \""
                           + shouldValue
                           + "\"";
            return false;
        }
    }

    return true;
}

//==================================================================================================
// PrintBlossom
//==================================================================================================
CmdBlossom::CmdBlossom()
    : Blossom()
{
    m_hasOutput = true;

    m_requiredKeys.insert("command", new Kitsunemimi::DataValue(true));
    m_requiredKeys.insert("ignore_errors", new Kitsunemimi::DataValue(false));
    m_requiredKeys.insert("trim_output", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
bool
CmdBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const std::string command = blossomItem.values.getValueAsString("command");
    bool ignoreResult = false;
    bool trimOutput = false;

    const Kitsunemimi::Sakura::ValueItem ignoreResultItem = blossomItem.values.getValueItem("ignore_errors");
    if(ignoreResultItem.item != nullptr)
    {
        if(ignoreResultItem.item->isBoolValue())
        {
            ignoreResult = ignoreResultItem.item->getBool();
        }
        else
        {
            errorMessage = "ignore_errors was set, but is not a bool-value";
            return false;
        }
    }

    const Kitsunemimi::Sakura::ValueItem trimOutputItem = blossomItem.values.getValueItem("trim_output");
    if(trimOutputItem.item != nullptr)
    {
        if(trimOutputItem.item->isBoolValue())
        {
            trimOutput = trimOutputItem.item->getBool();
        }
        else
        {
            errorMessage = "trim_output was set, but is not a bool-value";
            return false;
        }
    }

    LOG_DEBUG("run command: " + command);
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(command);

    LOG_DEBUG("command-output: \n" + processResult.processOutput);
    if(processResult.success == false
            && ignoreResult == false)
    {
        errorMessage = processResult.processOutput;
        return false;
    }

    if(trimOutput) {
        Kitsunemimi::trim(processResult.processOutput);
    }

    blossomItem.blossomOutput.insert("output",
                                     new Kitsunemimi::DataValue(processResult.processOutput));
    return true;
}

//==================================================================================================
// PrintBlossom
//==================================================================================================
ExitBlossom::ExitBlossom()
    : Blossom()
{
    m_requiredKeys.insert("status", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
bool
ExitBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    const int exitStatus = blossomItem.values.get("status")->toValue()->getInt();

    exit(exitStatus);
}

//==================================================================================================
// PrintBlossom
//==================================================================================================
ItemUpdateBlossom::ItemUpdateBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
bool
ItemUpdateBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::map<std::string, Kitsunemimi::Sakura::ValueItem>::const_iterator overrideIt;
    for(overrideIt = blossomItem.values.m_valueMap.begin();
        overrideIt != blossomItem.values.m_valueMap.end();
        overrideIt++)
    {
        std::map<std::string, DataItem*>::iterator originalIt;
        originalIt = blossomItem.parentValues->m_map.find(overrideIt->first);

        if(originalIt != blossomItem.parentValues->m_map.end()) {
            blossomItem.parentValues->insert(overrideIt->first,
                                             overrideIt->second.item->copy(),
                                             true);
        }
    }

    return true;
}

//==================================================================================================
// PrintBlossom
//==================================================================================================
PrintBlossom::PrintBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new Kitsunemimi::DataValue(false));
}

/**
 * runTask
 */
bool
PrintBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::string output = "";
    Kitsunemimi::TableItem tableItem;
    tableItem.addColumn("key", "Item-Name");
    tableItem.addColumn("value", "Value");

    std::map<std::string, Kitsunemimi::Sakura::ValueItem>::iterator it;
    for(it = blossomItem.values.m_valueMap.begin();
        it != blossomItem.values.m_valueMap.end();
        it++)
    {
        tableItem.addRow(std::vector<std::string>{it->first, it->second.item->toString(true)});
    }

    blossomItem.terminalOutput = tableItem.toString(150);

    return true;
}
