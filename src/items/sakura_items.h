/**
 * @file        sakura_items.h
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

#ifndef SAKURA_ITEMS_H
#define SAKURA_ITEMS_H

#include <common.h>
#include <items/value_items.h>

namespace SakuraTree
{

//===================================================================
// SakuraItem
//===================================================================
class SakuraItem
{
public:
    enum ItemType
    {
        UNDEFINED_ITEM = 0,
        BLOSSOM_ITEM = 1,
        BLOSSOM_GROUP_ITEM = 2,
        SUBTREE_ITEM = 3,
        TREE_ITEM = 4,
        SEED_ITEM = 5,
        SEQUENTIELL_ITEM = 6,
        PARALLEL_ITEM = 7,
        IF_ITEM = 8,
        FOR_EACH_ITEM = 9,
        FOR_ITEM = 10,
        PARALLEL_FOR_EACH_ITEM = 11,
        PARALLEL_FOR_ITEM = 12
    };

    SakuraItem();
    virtual ~SakuraItem();
    virtual SakuraItem* copy() = 0;

    ItemType getType() const;
    ValueItemMap values;

protected:
    ItemType type = UNDEFINED_ITEM;
};

//===================================================================
// BlossomItem
//===================================================================
class BlossomItem : public SakuraItem
{
public:
    enum BlossomStates
    {
        UNDEFINED_STATE = 0,
        CHANGED_STATE = 1,
        SKIPPED_STATE = 2,
        ERROR_INIT_STATE = 3,
        ERROR_PRECHECK_STATE = 4,
        ERROR_EXEC_STATE = 5,
        ERROR_POSTCHECK_STATE = 6,
        ERROR_CLOSE_STATE = 7
    };

    BlossomItem();
    ~BlossomItem();
    SakuraItem* copy();

    std::string blossomGroupType = "";
    std::string blossomType = "";
    std::string blossomPath = "";

    DataItem* blossomOutput = nullptr;
    DataMap* parentValues = nullptr;

    // process
    int execState = 0;
    std::string processOutput = "";
    std::string processError = "";

    // result
    BlossomStates resultState = UNDEFINED_STATE;
    std::vector<std::string> nameHirarchie;
    bool skip = false;
    bool success = true;
    std::string outputMessage = "";
};

//===================================================================
// BlossomGroupItem
//===================================================================
class BlossomGroupItem : public SakuraItem
{
public:
    BlossomGroupItem();
    ~BlossomGroupItem();
    SakuraItem* copy();

    std::string id = "";
    std::string blossomGroupType = "";

    std::vector<BlossomItem*> blossoms;
};

//===================================================================
// SeedItem
//===================================================================
class SeedItem : public SakuraItem
{
public:
    SeedItem();
    ~SeedItem();
    SakuraItem* copy();

    std::string name = "";
    std::string address = "";
    int sshPort = 0;
    std::string sshUser = "";
    std::string sshKey = "";
    std::string content = "";

    SakuraItem* child = nullptr;
};

//===================================================================
// BranchItem
//===================================================================
class SubtreeItem : public SakuraItem
{
public:
    SubtreeItem();
    ~SubtreeItem();
    SakuraItem* copy();

    std::string id = "";
    std::vector<SakuraItem*> childs;
};

//===================================================================
// IfBranching
//===================================================================
class IfBranching : public SakuraItem
{
public:
    enum compareTypes {
        EQUAL = 0,
        GREATER_EQUAL = 1,
        GREATER = 2,
        SMALLER_EQUAL = 3,
        SMALLER = 4,
        UNEQUAL = 5
    };

    IfBranching();
    ~IfBranching();
    SakuraItem* copy();

    ValueItem leftSide;
    compareTypes ifType = EQUAL;
    ValueItem rightSide;

    SakuraItem* ifContent;
    SakuraItem* elseContent;
};

//===================================================================
// ForEachBranching
//===================================================================
class ForEachBranching : public SakuraItem
{
public:
    ForEachBranching(const bool parallel=false);
    ~ForEachBranching();
    SakuraItem* copy();

    std::string tempVarName = "";
    ValueItemMap iterateArray;

    SakuraItem* content;
};

//===================================================================
// ForBranching
//===================================================================
class ForBranching : public SakuraItem
{
public:
    ForBranching(const bool parallel=false);
    ~ForBranching();
    SakuraItem* copy();

    std::string tempVarName = "";
    ValueItem start;
    ValueItem end;

    SakuraItem* content;
};

//===================================================================
// SequentiellPart
//===================================================================
class SequentiellPart : public SakuraItem
{
public:
    SequentiellPart();
    ~SequentiellPart();
    SakuraItem* copy();

    std::vector<SakuraItem*> childs;
};

//===================================================================
// ParallelPart
//===================================================================
class ParallelPart : public SakuraItem
{
public:
    ParallelPart();
    ~ParallelPart();
    SakuraItem* copy();

    std::vector<SakuraItem*> childs;
};

}  // namespace SakuraTree

#endif // SAKURA_ITEMS_H
