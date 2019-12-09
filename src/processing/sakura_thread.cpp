/**
 * @file        sakura_thread.cpp
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

#include "sakura_thread.h"

#include <sakura_root.h>
#include <items/item_methods.h>
#include <processing/blossoms/blossom.h>
#include <processing/blossoms/blossom_getter.h>

namespace SakuraTree
{

/**
 * constructor
 */
SakuraThread::SakuraThread(SakuraItem* growPlan,
                           ValueItemMap values,
                           const std::vector<std::string> &hierarchy)
{
    assert(growPlan != nullptr);

    m_values = values;
    m_growPlan = growPlan;
    m_hirarchie = hierarchy;
}

/**
 * destructor
 */
SakuraThread::~SakuraThread()
{
    clearChilds();
}

/**
 * wait until the thread was started
 */
void
SakuraThread::waitUntilStarted()
{
    while(m_started == false)
    {
        std::this_thread::sleep_for(chronoMicroSec(10));
    }
}

/**
 * check if the thread was aborted
 *
 * @return true, if thread was aborted, else false
 */
bool
SakuraThread::isAborted() const
{
    return m_abort;
}

/**
 * start thread
 */
void
SakuraThread::run()
{
    m_started = true;
    grow(m_growPlan, m_values, m_hirarchie);
}

/**
 * central method of the thread to process the current part of the execution-tree
 */
void
SakuraThread::grow(SakuraItem* growPlan,
                   ValueItemMap values,
                   const std::vector<std::string> &hierarchy)
{
    if(m_abort) {
        return;
    }

    if(growPlan->getType() == SakuraItem::BLOSSOM_ITEM)
    {
        BlossomItem* blossomItem = dynamic_cast<BlossomItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;

        // copy the item or else the for-each-loop is broken
        BlossomItem tempBlossomItem = *blossomItem;
        processBlossom(tempBlossomItem, values, newHierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::BLOSSOM_GROUP_ITEM)
    {
        BlossomGroupItem* blossomGroupItem = dynamic_cast<BlossomGroupItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        newHierarchy.push_back("BLOSSOM: " + blossomGroupItem->id);

        processBlossomGroup(*blossomGroupItem, values, newHierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::BRANCH_ITEM)
    {
        BranchItem* branchItem = dynamic_cast<BranchItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        newHierarchy.push_back("BRANCH: " + branchItem->id);

        processBranch(branchItem, values, newHierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::TREE_ITEM)
    {
        TreeItem* treeItem = dynamic_cast<TreeItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        newHierarchy.push_back("TREE: " + treeItem->id);

        processTree(treeItem, values, newHierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::IF_ITEM)
    {
        IfBranching* ifBranching = dynamic_cast<IfBranching*>(growPlan);
        processIf(ifBranching, values, hierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::FOR_EACH_ITEM)
    {
        ForEachBranching* forEachBranching = dynamic_cast<ForEachBranching*>(growPlan);
        processForEach(forEachBranching, values, hierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::FOR_ITEM)
    {
        ForBranching* forBranching = dynamic_cast<ForBranching*>(growPlan);
        processFor(forBranching, values, hierarchy);
        return;
    }

    if(growPlan->getType() == SakuraItem::SEQUENTIELL_ITEM)
    {
        SequentiellBranching* sequentiell = dynamic_cast<SequentiellBranching*>(growPlan);
        processSequeniellPart(sequentiell, values, hierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::PARALLEL_ITEM)
    {
        ParallelBranching* parallel = dynamic_cast<ParallelBranching*>(growPlan);
        processParallelPart(parallel, values, hierarchy);

        return;
    }

    if(growPlan->getType() == SakuraItem::SEED_ITEM)
    {
        SeedItem* forestItem = dynamic_cast<SeedItem*>(growPlan);
        BranchItem* branchItem = dynamic_cast<BranchItem*>(forestItem->child);
        processBranch(branchItem, values, hierarchy);

        return;
    }

    return;
}

/**
 * @brief SakuraThread::processBlossom
 */
void
SakuraThread::processBlossom(BlossomItem &growPlan,
                             ValueItemMap values,
                             const std::vector<std::string> &hierarchy)
{
    // init
    growPlan.nameHirarchie = hierarchy;

    // prepare values
    overrideItems(values, growPlan.parent->values);
    Result result = fillInputItems(growPlan.values, values);

    // check if value-preparation was successful
    if(result.success == false)
    {
        growPlan.outputMessage = result.errorMessage;
        growPlan.success = false;
        m_abort = true;
        return;
    }

    // get and process blossom
    Blossom* blossom = getBlossom(growPlan.blossomGroupType,
                                  growPlan.blossomType);
    blossom->growBlossom(growPlan);
    delete blossom;

    // abort if blossom-result was an error
    if(growPlan.success == false){
        m_abort = true;
    }

    // send result to root
    SakuraRoot::m_root->printOutput(growPlan);

    // write processing result back to parent
    fillOutputItems(growPlan.values, growPlan.blossomOutput);
    overrideItems(growPlan.parent->values, growPlan.values);

    return;
}

/**
 * @brief SakuraThread::processBlossomGroup
 * @param growPlan
 * @param values
 * @param hierarchy
 */
void
SakuraThread::processBlossomGroup(BlossomGroupItem &growPlan,
                                  ValueItemMap values,
                                  const std::vector<std::string> &hierarchy)
{
    for(uint32_t i = 0; i < growPlan.blossoms.size(); i++)
    {
        BlossomItem* blossomItem = growPlan.blossoms.at(i);
        blossomItem->blossomGroupType = growPlan.blossomGroupType;

        grow(blossomItem,
             values,
             hierarchy);
    }

    return;
}

/**
 * @brief SakuraThread::processBranch
 */
void
SakuraThread::processBranch(BranchItem* growPlan,
                            ValueItemMap values,
                            const std::vector<std::string> &hierarchy)
{
    Result result = fillInputItems(growPlan->values, values);
    if(result.success == false)
    {
        // TODO: error-output
        m_abort = true;
        return;
    }

    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             growPlan->values,
             hierarchy);
    }

    if(growPlan->parent != nullptr) {
        overrideItems(growPlan->parent->values, growPlan->values);
    }

    return;
}

/**
 * @brief SakuraThread::processTree
 */
void
SakuraThread::processTree(TreeItem* growPlan,
                          ValueItemMap values,
                          const std::vector<std::string> &hierarchy)
{
    Result result = fillInputItems(growPlan->values, values);
    if(result.success == false)
    {
        // TODO: error-output
        m_abort = true;
        return;
    }

    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             growPlan->values,
             hierarchy);
    }

    if(growPlan->parent != nullptr) {
        overrideItems(growPlan->parent->values, growPlan->values);
    }

    return;
}

/**
 * @brief SakuraThread::processIf
 * @param growPlan
 * @param values
 * @param hierarchy
 */
void
SakuraThread::processIf(IfBranching* growPlan,
                        ValueItemMap values,
                        const std::vector<std::string> &hierarchy)
{
    bool ifMatch = false;
    std::map<std::string, ValueItem>::iterator it;
    ValueItem valueItem;

    std::string leftSide = "";
    std::string rightSide = "";

    overrideItems(values, growPlan->parent->values);

    if(growPlan->leftSide.isIdentifier == true)
    {
        const std::string leftSideKey = growPlan->leftSide.item->toString();
        valueItem.item = values.getValueItem(leftSideKey).item->copy();
        valueItem.functions = growPlan->leftSide.functions;
        valueItem.item = valueItem.getProcessedItem();
        leftSide = valueItem.item->toString();
    }
    else
    {
        leftSide = growPlan->leftSide.item->toString();
    }

    if(growPlan->rightSide.isIdentifier == true)
    {
        const std::string rightSideKey = growPlan->rightSide.item->toString();
        valueItem.item = values.getValueItem(rightSideKey).item->copy();
        valueItem.functions = growPlan->rightSide.functions;
        valueItem.item = valueItem.getProcessedItem();
        rightSide = valueItem.item->toString();
    }
    else
    {
        rightSide = growPlan->rightSide.item->toString();
    }

    switch(growPlan->ifType)
    {
        case IfBranching::EQUAL:
            {
                ifMatch = leftSide == rightSide;
                break;
            }
        case IfBranching::UNEQUAL:
            {
                ifMatch = leftSide != rightSide;
                break;
            }
        default:
            // not implemented
            assert(false);
            break;
    }

    if(ifMatch)
    {
        for(uint32_t i = 0; i < growPlan->ifChilds.size(); i++)
        {
            grow(growPlan->ifChilds.at(i),
                 values,
                 hierarchy);
        }
    }
    else
    {
        for(uint32_t i = 0; i < growPlan->elseChilds.size(); i++)
        {
            grow(growPlan->elseChilds.at(i),
                 values,
                 hierarchy);
        }
    }
}

/**
 * @brief SakuraThread::processForEach
 * @param growPlan
 * @param values
 * @param hierarchy
 */
void
SakuraThread::processForEach(ForEachBranching* growPlan,
                             ValueItemMap values,
                             const std::vector<std::string> &hierarchy)
{
    overrideItems(values, growPlan->parent->values);

    Result result = fillInputItems(growPlan->iterateArray, values);
    if(result.success == false)
    {
        m_abort = true;
        return;
    }

    DataArray* array = growPlan->iterateArray.get("array")->toArray();
    for(uint32_t i = 0; i < array->size(); i++)
    {
        values.insert(growPlan->tempVarName, array->get(i), true);
        grow(growPlan->forChild.front(),
             values,
             hierarchy);
    }
}

/**
 * @brief SakuraThread::processFor
 * @param growPlan
 * @param values
 * @param hierarchy
 */
void
SakuraThread::processFor(ForBranching* growPlan,
                         ValueItemMap values,
                         const std::vector<std::string> &hierarchy)
{
    ValueItem valueItem;

    long startValue = 0;
    long endValue = 0;

    overrideItems(values, growPlan->parent->values);

    if(growPlan->start.isIdentifier == true)
    {
        const std::string rightSideKey = growPlan->start.item->toString();
        valueItem.item = values.getValueItem(rightSideKey).item->copy();
        valueItem.functions = growPlan->start.functions;
        valueItem.item = valueItem.getProcessedItem();
        startValue = valueItem.item->toValue()->getLong();
    }
    else
    {
        startValue = growPlan->start.item->toValue()->getLong();
    }

    if(growPlan->end.isIdentifier == true)
    {
        const std::string rightSideKey = growPlan->end.item->toString();
        valueItem.item = values.getValueItem(rightSideKey).item->copy();
        valueItem.functions = growPlan->end.functions;
        valueItem.item = valueItem.getProcessedItem();
        endValue = valueItem.item->toValue()->getLong();
    }
    else
    {
        endValue = growPlan->end.item->toValue()->getLong();
    }

    for(long i = startValue; i < endValue; i++)
    {
        values.insert(growPlan->tempVarName, new DataValue(i), true);
        grow(growPlan->forChild.front(),
             values,
             hierarchy);
    }
}

/**
 * @brief SakuraThread::processSequeniellPart
 */
void
SakuraThread::processSequeniellPart(SequentiellBranching* growPlan,
                                    ValueItemMap values,
                                    const std::vector<std::string> &hierarchy)
{
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             values,
             hierarchy);
    }

    return;
}

/**
 * @brief SakuraThread::processParallelPart
 */
void
SakuraThread::processParallelPart(ParallelBranching* growPlan,
                                  ValueItemMap values,
                                  const std::vector<std::string> &hierarchy)
{
    // create and initialize all threads
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        SakuraThread* child = new SakuraThread(growPlan->childs.at(i),
                                               values,
                                               hierarchy);
        m_childs.push_back(child);
        child->startThread();
    }

    // wait for the end of all threads
    for(uint32_t i = 0; i < m_childs.size(); i++)
    {
        // wait the process is startet, so the join-method can not be called
        // before the new thread was really started
        m_childs.at(i)->waitUntilStarted();

        // finish the threads after they done their tasks
        m_childs.at(i)->waitForFinish();

        // check if any child was aborted
        if(m_childs.at(i)->isAborted()) {
            m_abort = true;
        }
    }
    clearChilds();

    return;
}

/**
 * stop and delete all child processes
 */
void
SakuraThread::clearChilds()
{
    for(uint32_t i = 0; i < m_childs.size(); i++)
    {
        m_childs.at(i)->stopThread();
        delete m_childs[i];
    }
    m_childs.clear();
}

}
