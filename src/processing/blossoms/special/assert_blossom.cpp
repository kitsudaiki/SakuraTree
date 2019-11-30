/**
 * @file        assert_blossom.cpp
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

#include "assert_blossom.h"
#include <libKitsunemimiCommon/common_items/table_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

AssertBlossom::AssertBlossom() :
    Blossom() {}

/**
 * initTask
 */
void
AssertBlossom::initTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
AssertBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
AssertBlossom::runTask(BlossomItem &blossomItem)
{
    const std::vector<std::string> keys = blossomItem.inputValues.getKeys();
    for(uint32_t i = 0; i < keys.size(); i++)
    {
        std::string isValue = "";
        if(blossomItem.parent->inputValues.contains(keys.at(i))) {
            isValue = blossomItem.parent->inputValues.get(keys.at(i))->toString();
        }
        if(blossomItem.parent->outputValues.contains(keys.at(i))) {
            isValue = blossomItem.parent->outputValues.get(keys.at(i))->toString();
        }

        const std::string shouldValue = blossomItem.inputValues.get(keys.at(i))->toString();

        if(isValue != shouldValue)
        {
            blossomItem.success = false;
            blossomItem.outputMessage = "ASSERT FAILES: the variable \""
                                        + keys.at(i)
                                        + "\" has the value \""
                                        + isValue
                                        + "\", but it should have the value \""
                                        + shouldValue
                                        + "\"";

        }
    }
}

/**
 * postCheck
 */
void
AssertBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
AssertBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
