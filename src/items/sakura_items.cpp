/**
 * @file        sakura_items.cpp
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

#include <items/sakura_items.h>

namespace SakuraTree
{

//===================================================================
// SakuraItem
//===================================================================
SakuraItem::SakuraItem() {}

SakuraItem::~SakuraItem() {}

SakuraItem::ItemType SakuraItem::getType() const
{
    return type;
}

//===================================================================
// BlossomItem
//===================================================================
BlossomItem::BlossomItem()
{
    type = BLOSSOM_ITEM;
}

BlossomItem::~BlossomItem()
{
    if(blossomOutput != nullptr) {
        delete blossomOutput;
    }
}

//===================================================================
// BlossomGroupItem
//===================================================================
BlossomGroupItem::BlossomGroupItem()
{
    type = BLOSSOM_GROUP_ITEM;
}

BlossomGroupItem::~BlossomGroupItem() {}


//===================================================================
// BranchItem
//===================================================================
BranchItem::BranchItem()
{
    type = BRANCH_ITEM;
}

BranchItem::~BranchItem()
{
    for(uint32_t i = 0; i < childs.size(); i++)
    {
        SakuraItem* tempItem = childs.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

//===================================================================
// SeedItem
//===================================================================
SeedItem::SeedItem()
{
    type = SEED_ITEM;
}

SeedItem::~SeedItem()
{
    if(child != nullptr) {
        delete child;
    }
}

//===================================================================
// TreeItem
//===================================================================
TreeItem::TreeItem()
{
    type = TREE_ITEM;
}

TreeItem::~TreeItem()
{
    for(uint32_t i = 0; i < childs.size(); i++)
    {
        SakuraItem* tempItem = childs.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

//===================================================================
// Sequeniell
//===================================================================
SequeniellBranching::SequeniellBranching()
{
    type = SEQUENTIELL_ITEM;
}

SequeniellBranching::~SequeniellBranching() {}

//===================================================================
// ParallelBranching
//===================================================================
ParallelBranching::ParallelBranching()
{
    type = PARALLEL_ITEM;
}

ParallelBranching::~ParallelBranching() {}

//===================================================================
// IfBranching
//===================================================================
IfBranching::IfBranching()
{
    type = IF_ITEM;
}

IfBranching::~IfBranching()
{
    if(leftSide != nullptr) {
        delete leftSide;
    }
    if(rightSide != nullptr) {
        delete rightSide;
    }

    for(uint32_t i = 0; i < ifChilds.size(); i++)
    {
        SakuraItem* tempItem = ifChilds.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }

    for(uint32_t i = 0; i < elseChilds.size(); i++)
    {
        SakuraItem* tempItem = elseChilds.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

//===================================================================
// ForEachBranching
//===================================================================
ForEachBranching::ForEachBranching()
{
    type = FOR_EACH_ITEM;
}

ForEachBranching::~ForEachBranching()
{
    for(uint32_t i = 0; i < forChild.size(); i++)
    {
        SakuraItem* tempItem = forChild.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

//===================================================================
// ForBranching
//===================================================================
ForBranching::ForBranching()
{
    type = FOR_ITEM;
}

ForBranching::~ForBranching()
{
    for(uint32_t i = 0; i < forChild.size(); i++)
    {
        SakuraItem* tempItem = forChild.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

}  // namespace SakuraTree
