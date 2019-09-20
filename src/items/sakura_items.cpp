/**
 *  @file    sakura_items.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
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
BlossomItem::BlossomItem() {type = BLOSSOM_ITEM;}

BlossomItem::~BlossomItem() {}

//===================================================================
// BranchItem
//===================================================================
BranchItem::BranchItem() {type = BRANCH_ITEM;}

BranchItem::~BranchItem() {}

//===================================================================
// TreeItem
//===================================================================
TreeItem::TreeItem() {type = TREE_ITEM;}

TreeItem::~TreeItem() {}

//===================================================================
// Sequeniell
//===================================================================
SequeniellBranching::SequeniellBranching() {type = SEQUENTIELL_ITEM;}

SequeniellBranching::~SequeniellBranching() {}

//===================================================================
// ParallelBranching
//===================================================================
ParallelBranching::ParallelBranching() {type = PARALLEL_ITEM;}

ParallelBranching::~ParallelBranching() {}

//===================================================================
// IfBranching
//===================================================================
IfBranching::IfBranching() {type = IF_ITEM;}

IfBranching::~IfBranching() {}

//===================================================================
// ForBranching
//===================================================================
ForBranching::ForBranching() {type = FOR_ITEM;}

ForBranching::~ForBranching() {}

}  // namespace SakuraTree
