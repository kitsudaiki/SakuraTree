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
    allowUnmatched = true;
}

/**
 * runTask
 */
bool
AssertBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    std::map<std::string, Kitsunemimi::DataItem*>::iterator it;
    for(it = blossomLeaf.input.m_map.begin();
        it != blossomLeaf.input.m_map.end();
        it++)
    {
        const std::string isValue = blossomLeaf.parentValues->get(it->first)->toString();
        const std::string shouldValue = it->second->toString();

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
    validationMap.emplace("command", BlossomValidDef(IO_ValueType::INPUT_TYPE, true));
    validationMap.emplace("ignore_errors", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
    validationMap.emplace("trim_output", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
    validationMap.emplace("output", BlossomValidDef(IO_ValueType::OUTPUT_TYPE, false));
}

/**
 * runTask
 */
bool
CmdBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const std::string command = blossomLeaf.input.getStringByKey("command");
    bool ignoreResult = false;
    bool trimOutput = false;

    Kitsunemimi::DataItem* ignoreResultItem = blossomLeaf.input.get("ignore_errors");
    if(ignoreResultItem != nullptr) {
        ignoreResult = ignoreResultItem->toValue()->getBool();
    }

    Kitsunemimi::DataItem* trimOutputItem = blossomLeaf.input.get("trim_output");
    if(trimOutputItem != nullptr) {
        trimOutput = trimOutputItem->toValue()->getBool();
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

    blossomLeaf.output.insert("output", new Kitsunemimi::DataValue(processResult.processOutput));

    return true;
}

//==================================================================================================
// PrintBlossom
//==================================================================================================
ExitBlossom::ExitBlossom()
    : Blossom()
{
    validationMap.emplace("status", BlossomValidDef(IO_ValueType::INPUT_TYPE, false));
}

/**
 * runTask
 */
bool
ExitBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    const int exitStatus = blossomLeaf.input.get("status")->toValue()->getInt();

    exit(exitStatus);
}

//==================================================================================================
// PrintBlossom
//==================================================================================================
ItemUpdateBlossom::ItemUpdateBlossom()
    : Blossom()
{
    allowUnmatched = true;
}

/**
 * runTask
 */
bool
ItemUpdateBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    std::map<std::string, Kitsunemimi::DataItem*>::iterator it;
    for(it = blossomLeaf.input.m_map.begin();
        it != blossomLeaf.input.m_map.end();
        it++)
    {
        std::map<std::string, DataItem*>::iterator originalIt;
        originalIt = blossomLeaf.parentValues->m_map.find(it->first);

        if(originalIt != blossomLeaf.parentValues->m_map.end())
        {
            blossomLeaf.parentValues->insert(it->first,
                                             it->second->copy(),
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
    allowUnmatched = true;
}

/**
 * runTask
 */
bool
PrintBlossom::runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage)
{
    std::string output = "";
    Kitsunemimi::TableItem tableItem;
    tableItem.addColumn("key", "Item-Name");
    tableItem.addColumn("value", "Value");

    std::map<std::string, Kitsunemimi::DataItem*>::iterator it;
    for(it = blossomLeaf.input.m_map.begin();
        it != blossomLeaf.input.m_map.end();
        it++)
    {
        tableItem.addRow(std::vector<std::string>{it->first, it->second->toString(true)});
    }

    blossomLeaf.terminalOutput = tableItem.toString(150);

    return true;
}
