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
SubtreeItem::SubtreeItem()
{
    type = SUBTREE_ITEM;
}

SubtreeItem::~SubtreeItem()
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
Sequentiell::Sequentiell()
{
    type = SEQUENTIELL_ITEM;
}

Sequentiell::~Sequentiell()
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
// IfBranching
//===================================================================
IfBranching::IfBranching()
{
    type = IF_ITEM;
}

IfBranching::~IfBranching()
{
    if(ifContent != nullptr) {
        delete ifContent;
    }

    if(elseContent != nullptr) {
        delete elseContent;
    }
}

//===================================================================
// ForEachBranching
//===================================================================
ForEachBranching::ForEachBranching(const bool parallel)
{
    if(parallel) {
        type = PARALLEL_FOR_EACH_ITEM;
    } else {
        type = FOR_EACH_ITEM;
    }
}

ForEachBranching::~ForEachBranching()
{
    if(content != nullptr) {
        delete content;
    }
}

//===================================================================
// ForBranching
//===================================================================
ForBranching::ForBranching(const bool parallel)
{
    if(parallel) {
        type = PARALLEL_FOR_ITEM;
    } else {
        type = FOR_ITEM;
    }
}

ForBranching::~ForBranching()
{
    if(content != nullptr) {
        delete content;
    }
}

//===================================================================
// Parallel
//===================================================================
Parallel::Parallel()
{
    type = PARALLEL_ITEM;
}

Parallel::~Parallel()
{
    for(uint32_t i = 0; i < childs.size(); i++)
    {
        SakuraItem* tempItem = childs.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

}  // namespace SakuraTree
