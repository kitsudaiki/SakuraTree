/**
 * @file        blossom.cpp
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

#include "blossom.h"
#include <sakura_root.h>
#include <items/item_methods.h>

namespace SakuraTree
{

Blossom::Blossom() {}

Blossom::~Blossom() {}

/**
 * @brief SakuraBlossom::growBlossom
 * @return
 */
void
Blossom::growBlossom(BlossomItem &blossomItem)
{
    const std::vector<std::string> uninitItems = checkItems(blossomItem.values);
    if(uninitItems.size() > 0)
    {
        std::string output = "The following items are not initialized: \n";
        for(uint32_t i = 0; i < uninitItems.size(); i++)
        {
            output += uninitItems.at(i) + "\n";
        }
        blossomItem.outputMessage = output;
        blossomItem.success = false;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"initTask"<<std::endl;
    }
    initTask(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_INIT_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"preCheck"<<std::endl;
    }
    preCheck(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_PRECHECK_STATE;
        return;
    }

    if(blossomItem.skip)
    {
        blossomItem.resultState = BlossomItem::SKIPPED_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"runTask"<<std::endl;
    }
    runTask(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_EXEC_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"postCheck"<<std::endl;
    }
    postCheck(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_POSTCHECK_STATE;

        return;
    }
    //-------------------------------
    if(DEBUG) {
        std::cout<<"closeTask"<<std::endl;
    }
    closeTask(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_CLOSE_STATE;
        return;
    }

    //-------------------------------

    blossomItem.resultState = BlossomItem::CHANGED_STATE;
    return;
}

}
