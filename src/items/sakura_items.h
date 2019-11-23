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

#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiCommon/common_items/data_items.h>

using Kitsunemimi::Common::DataItem;
using Kitsunemimi::Common::DataMap;
using Kitsunemimi::Common::DataArray;
using Kitsunemimi::Common::DataValue;

using Kitsunemimi::Json::JsonItem;

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
        BRANCH_ITEM = 3,
        TREE_ITEM = 4,
        SEED_ITEM = 5,
        SEQUENTIELL_ITEM = 6,
        PARALLEL_ITEM = 7,
        IF_ITEM = 8,
        FOR_ITEM = 9
    };

    SakuraItem();
    virtual ~SakuraItem();

    SakuraItem* parent = nullptr;

    ItemType getType() const;

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

    DataMap inputValues;
    DataMap outputValue;

    std::string blossomGroupType = "";
    std::string blossomType = "";

    // result
    BlossomStates resultState = UNDEFINED_STATE;
    std::vector<std::string> nameHirarchie;
    bool skip = false;
    bool success = true;
    int execState = 0;
    std::string errorMessage = "";
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

    std::string name = "";
    std::string address = "";
    int sshPort = 0;
    std::string sshUser = "";
    std::string sshKey = "";
    std::string content = "";

    SakuraItem* child = nullptr;
};

//===================================================================
// TreeItem
//===================================================================
class TreeItem : public SakuraItem
{
public:
    TreeItem();
    ~TreeItem();

    std::string id = "";
    DataMap values;
    std::vector<SakuraItem*> childs;
};

//===================================================================
// BranchItem
//===================================================================
class BranchItem : public SakuraItem
{
public:
    BranchItem();
    ~BranchItem();

    std::string id = "";
    DataMap values;
    std::vector<SakuraItem*> childs;
};

//===================================================================
// Sequeniell
//===================================================================
class SequeniellBranching : public SakuraItem
{
public:
    SequeniellBranching();
    ~SequeniellBranching();

    std::vector<SakuraItem*> childs;
};

//===================================================================
// ParallelBranching
//===================================================================
class ParallelBranching : public SakuraItem
{
public:
    ParallelBranching();
    ~ParallelBranching();

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

    DataArray* leftSide = nullptr;
    compareTypes ifType = EQUAL;
    DataValue* rightSide = nullptr;

    std::vector<SakuraItem*> ifChilds;
    std::vector<SakuraItem*> elseChilds;
};

//===================================================================
// ForBranching
//===================================================================
class ForBranching : public SakuraItem
{
public:
    ForBranching();
    ~ForBranching();

    std::string tempVarName = "";
    DataArray iterateArray;

    SakuraItem* forChild = nullptr;
};


}  // namespace SakuraTree

#endif // SAKURA_ITEMS_H
