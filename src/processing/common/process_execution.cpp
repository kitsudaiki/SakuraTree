/**
 * @file        process_execution.cpp
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

#include "process_execution.h"
#include <boost/algorithm/string/replace.hpp>

namespace SakuraTree
{

/**
 * @brief run a programm synchrone
 *
 * @param blossomItem blossom-item, where the results of the run should be written back
 * @param command command to execute as shell-command
 *
 * @return false, if execution failed, else true
 */
bool
runSyncProcess(BlossomItem &blossomItem,
               std::string command)
{
    std::vector<std::string> args;
    args.push_back("-c");
    boost::replace_all(command, "\"", "\\\"");
    args.push_back("\"" + command + "\"");

    return runSyncProcess(blossomItem, std::string("/bin/sh"), args);
}

/**
 * @brief run a programm synchrone
 *
 * @param blossomItem blossom-item, where the results of the run should be written back
 * @param programm path to the programm to execute
 * @param args list of arguments
 *
 * @return false, if execution failed, else true
 */
bool
runSyncProcess(BlossomItem &blossomItem,
               const std::string &programm,
               const std::vector<std::string> &args)
{
    std::string call = programm;
    for(uint32_t i = 0; i < args.size(); i++)
    {
        call += " " + args[i];
    }
    call.append(" 2>&1");

    if(DEBUG) {
        std::cout<<"call: "<<call<<std::endl;
    }

    FILE* stream = nullptr;

    const uint32_t max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(call.c_str(), "r");

    if(stream)
    {
        while(!feof(stream))
        {
            if(fgets(buffer, max_buffer, stream) != nullptr) {
                blossomItem.processOutput.append(buffer);
            }
        }
        blossomItem.execState = pclose(stream);
    }
    else
    {
        blossomItem.outputMessage = "can not execute programm: " + programm;
        blossomItem.success = false;

        return false;
    }

    if(DEBUG)
    {
        std::cout<<"+++++++++++++++++++++++++++++++++++++++ exit status: "
                 <<blossomItem.execState
                 <<std::endl;

        if(blossomItem.processOutput.size() < 10000){
            std::cout<<blossomItem.processOutput<<std::endl;
        }
    }

    if(blossomItem.execState != 0)
    {
        blossomItem.success = false;
        return false;
    }

    blossomItem.success = true;
    return true;
}

}
