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
#include <libKitsunemimiCommon/common_items/table_item.h>
#include <libKitsunemimiPersistence/files/text_file.h>

namespace SakuraTree
{

PrintBlossom::PrintBlossom()
    : Blossom()
{
    m_requiredKeys.insert("*", new DataValue(false));
}

/**
 * initBlossom
 */
void
PrintBlossom::initBlossom(BlossomItem &blossomItem)
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
    Kitsunemimi::TableItem tableItem;
    tableItem.addColumn("key", "Item-Name");
    tableItem.addColumn("value", "Value");

    std::map<std::string, ValueItem>::iterator it;
    for(it = blossomItem.values.m_valueMap.begin();
        it != blossomItem.values.m_valueMap.end();
        it++)
    {
        tableItem.addRow(std::vector<std::string>{it->first, it->second.item->toString(true)});
    }

    blossomItem.outputMessage = tableItem.toString(150);
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
 * closeBlossom
 */
void
PrintBlossom::closeBlossom(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
