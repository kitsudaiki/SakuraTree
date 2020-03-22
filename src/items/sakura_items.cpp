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

SakuraItem*
BlossomItem::copy()
{
    BlossomItem* newItem = new BlossomItem();

    newItem->type = type;
    newItem->values = values;

    newItem->blossomName = blossomName;
    newItem->blossomGroupType = blossomGroupType;
    newItem->blossomType = blossomType;
    newItem->blossomPath = blossomPath;

    if(blossomOutput != nullptr) {
        newItem->blossomOutput = blossomOutput->copy();
    }
    if(parentValues != nullptr) {
        newItem->parentValues = parentValues;
    }

    // process
    newItem->processResult.success = processResult.success;
    newItem->processResult.exitStatus = processResult.exitStatus;
    newItem->processResult.errorMessage = processResult.errorMessage;
    newItem->processResult.processOutput = processResult.processOutput;

    // result
    newItem->nameHirarchie = nameHirarchie;
    newItem->skip = skip;
    newItem->success = success;
    newItem->outputMessage = outputMessage;

    return newItem;
}

//===================================================================
// BlossomGroupItem
//===================================================================
BlossomGroupItem::BlossomGroupItem()
{
    type = BLOSSOM_GROUP_ITEM;
}

BlossomGroupItem::~BlossomGroupItem()
{
    for(uint32_t i = 0; i < blossoms.size(); i++)
    {
        SakuraItem* tempItem = blossoms.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

SakuraItem*
BlossomGroupItem::copy()
{
    BlossomGroupItem* newItem = new BlossomGroupItem();

    newItem->type = type;
    newItem->values = values;

    newItem->id = id;
    newItem->blossomGroupType = blossomGroupType;

    for(uint32_t i = 0; i < blossoms.size(); i++)
    {
        newItem->blossoms.push_back(static_cast<BlossomItem*>(blossoms.at(i)->copy()));
    }

    return newItem;
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

SakuraItem*
TreeItem::copy()
{
    TreeItem* newItem = new TreeItem();

    newItem->type = type;
    newItem->values = values;

    newItem->id = id;
    for(uint32_t i = 0; i < childs.size(); i++)
    {
        newItem->childs.push_back(childs.at(i)->copy());
    }

    return newItem;
}

//===================================================================
// SubtreeItem
//===================================================================
SubtreeItem::SubtreeItem()
{
    type = SUBTREE_ITEM;
}

SubtreeItem::~SubtreeItem() {}

SakuraItem*
SubtreeItem::copy()
{
    SubtreeItem* newItem = new SubtreeItem();

    newItem->type = type;
    newItem->values = values;

    newItem->nameOrPath = nameOrPath;
    newItem->internalSubtrees = internalSubtrees;

    return newItem;
}

//===================================================================
// SeedItem
//===================================================================
SeedItem::SeedItem()
{
    type = SEED_ITEM;
}

SeedItem::~SeedItem() {}

SakuraItem*
SeedItem::copy()
{
    SeedItem* newItem = new SeedItem();

    newItem->type = type;
    newItem->values = values;

    newItem->treeId = treeId;
    newItem->tag = tag;

    return newItem;
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

SakuraItem*
IfBranching::copy()
{
    IfBranching* newItem = new IfBranching();

    newItem->type = type;
    newItem->values = values;

    newItem->leftSide = leftSide;
    newItem->ifType = ifType;
    newItem->rightSide = rightSide;

    if(ifContent != nullptr) {
        newItem->ifContent = ifContent->copy();
    }
    if(elseContent != nullptr) {
        newItem->elseContent = elseContent->copy();
    }

    return newItem;
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

SakuraItem*
ForEachBranching::copy()
{
    ForEachBranching* newItem = new ForEachBranching();

    newItem->type = type;
    newItem->values = values;

    newItem->tempVarName = tempVarName;
    newItem->iterateArray = iterateArray;
    if(content != nullptr) {
        newItem->content = content->copy();
    }

    return newItem;
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

SakuraItem*
ForBranching::copy()
{
    ForBranching* newItem = new ForBranching();

    newItem->type = type;
    newItem->values = values;

    newItem->tempVarName = tempVarName;
    newItem->start = start;
    newItem->end = end;

    if(content != nullptr) {
        newItem->content = content->copy();
    }

    return newItem;
}

//===================================================================
// SequentiellPart
//===================================================================
SequentiellPart::SequentiellPart()
{
    type = SEQUENTIELL_ITEM;
}

SequentiellPart::~SequentiellPart()
{
    for(uint32_t i = 0; i < childs.size(); i++)
    {
        SakuraItem* tempItem = childs.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

SakuraItem*
SequentiellPart::copy()
{
    SequentiellPart* newItem = new SequentiellPart();

    newItem->type = type;
    newItem->values = values;

    for(uint32_t i = 0; i < childs.size(); i++)
    {
        newItem->childs.push_back(childs.at(i)->copy());
    }

    return newItem;
}

//===================================================================
// ParallelPart
//===================================================================
ParallelPart::ParallelPart()
{
    type = PARALLEL_ITEM;
}

ParallelPart::~ParallelPart()
{
    for(uint32_t i = 0; i < childs.size(); i++)
    {
        SakuraItem* tempItem = childs.at(i);
        if(tempItem != nullptr) {
            delete tempItem;
        }
    }
}

SakuraItem*
ParallelPart::copy()
{
    ParallelPart* newItem = new ParallelPart();

    newItem->type = type;
    newItem->values = values;

    for(uint32_t i = 0; i < childs.size(); i++)
    {
        newItem->childs.push_back(childs.at(i)->copy());
    }

    return newItem;
}

}  // namespace SakuraTree
