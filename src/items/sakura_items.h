/**
 *  @file    sakura_items.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURA_ITEMS_H
#define SAKURA_ITEMS_H

#include <iostream>
#include <libKitsunemimiCommon/common_items/data_items.h>

using Kitsunemimi::Common::DataItem;
using Kitsunemimi::Common::DataMap;
using Kitsunemimi::Common::DataArray;
using Kitsunemimi::Common::DataValue;

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
        BRANCH_ITEM = 2,
        TREE_ITEM = 3,
        SEQUENTIELL_ITEM = 4,
        PARALLEL_ITEM = 5,
        IF_ITEM = 6,
        FOR_ITEM = 7
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

    std::string name = "";
    DataMap settings;
    DataMap values;

    std::string blossomType = "";
    std::vector<std::string> blossomSubTypes;

    // result
    BlossomStates resultState = UNDEFINED_STATE;
    std::vector<std::string> nameHirarchie;
    bool skip = false;
    bool success = true;
    int execState = 0;
    std::string outputMessage = "";
};

//===================================================================
// TreeItem
//===================================================================
class TreeItem : public SakuraItem
{
public:
    TreeItem();
    ~TreeItem();

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

    std::string leftSide = "";
    compareTypes ifType = EQUAL;
    DataValue rightSide;

    SakuraItem* ifChild = nullptr;
    SakuraItem* elseChild = nullptr;
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
