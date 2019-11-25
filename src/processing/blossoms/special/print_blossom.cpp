/**
 * @file        print_blossom.cpp
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

#include "print_blossom.h"

namespace SakuraTree
{

PrintBlossom::PrintBlossom() :
    Blossom() {}

/**
 * initTask
 */
void
PrintBlossom::initTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
PrintBlossom::preCheck(BlossomItem &blossomItem)
{
    // TODO: check that at least one item is there to be printed
    blossomItem.success = true;
}

/**
 * runTask
 */
void
PrintBlossom::runTask(BlossomItem &blossomItem)
{
    std::string output = "";
    const std::vector<std::string> keys = blossomItem.inputValues.getKeys();

    for(uint32_t i = 0; i < keys.size(); i++)
    {
        output += keys.at(i)
                + ": \n"
                + blossomItem.inputValues.get(keys.at(i))->toString()
                + "\n";
    }

    blossomItem.outputMessage = output;
}

/**
 * postCheck
 */
void
PrintBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
PrintBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
