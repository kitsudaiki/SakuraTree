/**
 * @file        cmd_blossom.cpp
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

#include "cmd_blossom.h"

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/process_execution.h>

/**
 * @brief constructor
 */
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
void
CmdBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
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
            blossomItem.success = true;
        }
        else
        {
            const std::string errorMsg = "ignore_errors was set, but is not a bool-value";
            blossomItem.outputMessage = errorMsg;
            blossomItem.success = false;
        }
    }

    const Kitsunemimi::Sakura::ValueItem trimOutputItem = blossomItem.values.getValueItem("trim_output");
    if(trimOutputItem.item != nullptr)
    {
        if(trimOutputItem.item->isBoolValue())
        {
            trimOutput = trimOutputItem.item->getBool();
            blossomItem.success = true;
        }
        else
        {
            const std::string errorMsg = "trim_output was set, but is not a bool-value";
            blossomItem.outputMessage = errorMsg;
            blossomItem.success = false;
        }
    }


    LOG_DEBUG("run command: " + command);
    Kitsunemimi::ProcessResult processResult = Kitsunemimi::runSyncProcess(command);
    blossomItem.success = processResult.success;

    if(ignoreResult) {
        blossomItem.success = true;
    }

    LOG_DEBUG("command-output: \n" + processResult.processOutput);
    if(blossomItem.success == false) {
        blossomItem.outputMessage = processResult.processOutput;
    }

    if(trimOutput) {
        Kitsunemimi::trim(processResult.processOutput);
    }

    blossomItem.blossomOutput.insert("output",
                                     new Kitsunemimi::DataValue(processResult.processOutput));
}
