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
                           DataMap &values,
                           const std::vector<std::string> &hierarchy)
{
    assert(growPlan != nullptr);

    m_values = values;
    m_growPlan = growPlan;
    m_hirarchie = hierarchy;
    m_returnValues = DataMap();
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
    grow(m_growPlan, m_values, m_hirarchie, m_returnValues);
}

/**
 * central method of the thread to process the current part of the execution-tree
 */
void
SakuraThread::grow(SakuraItem* growPlan,
                   DataMap &values,
                   const std::vector<std::string> &hierarchy,
                   DataMap &returnValues)
{
    if(m_abort) {
        return;
    }

    if(growPlan->getType() == SakuraItem::BLOSSOM_ITEM)
    {
        BlossomItem* blossomItem = dynamic_cast<BlossomItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        overrideItems(values, returnValues);

        std::pair<bool, std::string> result = fillItems(blossomItem->inputValues, values);
        if(result.first == false)
        {
            blossomItem->errorMessage = result.second;
            blossomItem->success = false;
            m_abort = true;
            return;
        }

        processBlossom(*blossomItem,
                       blossomItem->inputValues,
                       newHierarchy,
                       returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::BLOSSOM_GROUP_ITEM)
    {
        BlossomGroupItem* blossomGroupItem = dynamic_cast<BlossomGroupItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        newHierarchy.push_back("BLOSSOM: " + blossomGroupItem->id);
        processBlossomGroup(*blossomGroupItem,
                            values,
                            newHierarchy,
                            returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::BRANCH_ITEM)
    {
        BranchItem* branchItem = dynamic_cast<BranchItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        newHierarchy.push_back("BRANCH: " + branchItem->id);

        std::pair<bool, std::string> result = fillItems(branchItem->values, values);
        if(result.first == false)
        {
            // TODO: error-output
            m_abort = true;
            return;
        }

        processBranch(branchItem,
                      branchItem->values,
                      newHierarchy,
                      returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::TREE_ITEM)
    {
        TreeItem* treeItem = dynamic_cast<TreeItem*>(growPlan);
        std::vector<std::string> newHierarchy = hierarchy;
        newHierarchy.push_back("TREE: " + treeItem->id);

        std::pair<bool, std::string> result = fillItems(treeItem->values, values);
        if(result.first == false)
        {
            // TODO: error-output
            m_abort = true;
            return;
        }

        processTree(treeItem,
                    treeItem->values,
                    newHierarchy,
                    returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::IF_ITEM)
    {
        processIf(dynamic_cast<IfBranching*>(growPlan),
                  values,
                  hierarchy,
                  returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::SEQUENTIELL_ITEM)
    {
        processSequeniellPart(dynamic_cast<SequeniellBranching*>(growPlan),
                              values,
                              hierarchy,
                              returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::PARALLEL_ITEM)
    {
        processParallelPart(dynamic_cast<ParallelBranching*>(growPlan),
                            values,
                            hierarchy,
                            returnValues);
        return;
    }

    if(growPlan->getType() == SakuraItem::SEED_ITEM)
    {
        SeedItem* forestItem = dynamic_cast<SeedItem*>(growPlan);
        BranchItem* branchItem = dynamic_cast<BranchItem*>(forestItem->child);
        processBranch(branchItem,
                      values,
                      hierarchy,
                      returnValues);
    }

    return;
}

/**
 * @brief SakuraThread::processBlossom
 */
void
SakuraThread::processBlossom(BlossomItem &growPlan,
                             DataMap &values,
                             const std::vector<std::string> &hierarchy,
                             DataMap &returnValues)
{
    // init
    growPlan.inputValues = *(values.copy()->toMap());
    growPlan.nameHirarchie = hierarchy;

    Blossom* blossom = getBlossom(growPlan.blossomGroupType,
                                  growPlan.blossomType);
    blossom->growBlossom(growPlan);
    delete blossom;

    // abort if blossom-result was an error
    if(growPlan.success == false){
        m_abort = true;
    }

    overrideItems(returnValues, growPlan.outputValue);

    // send result to root
    SakuraRoot::m_root->printOutput(growPlan);

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
                                  DataMap &values,
                                  const std::vector<std::string> &hierarchy,
                                  DataMap &returnValues)
{
    for(uint32_t i = 0; i < growPlan.blossoms.size(); i++)
    {
        BlossomItem* blossomItem = growPlan.blossoms.at(i);
        blossomItem->blossomGroupType = growPlan.blossomGroupType;

        grow(blossomItem,
             values,
             hierarchy,
             returnValues);
    }

    return;
}

/**
 * @brief SakuraThread::processBranch
 */
void
SakuraThread::processBranch(BranchItem* growPlan,
                            DataMap &values,
                            const std::vector<std::string> &hierarchy,
                            DataMap &returnValues)
{
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             values,
             hierarchy,
             returnValues);
    }

    return;
}

/**
 * @brief SakuraThread::processTree
 */
void
SakuraThread::processTree(TreeItem* growPlan,
                          DataMap &values,
                          const std::vector<std::string> &hierarchy,
                          DataMap &returnValues)
{
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             values,
             hierarchy,
             returnValues);
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
                        DataMap &values,
                        const std::vector<std::string> &hierarchy,
                        DataMap &returnValues)
{
    bool ifMatch = false;

    switch(growPlan->ifType)
    {
        case IfBranching::EQUAL:
            {
                const std::string key = growPlan->leftSide->get(0)->toString();
                ifMatch = values.get(key)->toString() == growPlan->rightSide->toString();
                break;
            }
        case IfBranching::UNEQUAL:
            {
                const std::string key = growPlan->leftSide->get(0)->toString();
                ifMatch = values.get(key)->toString() != growPlan->rightSide->toString();
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
                 hierarchy,
                 returnValues);
        }
    }
    else
    {
        for(uint32_t i = 0; i < growPlan->elseChilds.size(); i++)
        {
            grow(growPlan->elseChilds.at(i),
                 values,
                 hierarchy,
                 returnValues);
        }
    }
}

/**
 * @brief SakuraThread::processSequeniellPart
 */
void
SakuraThread::processSequeniellPart(SequeniellBranching* growPlan,
                                    DataMap &values,
                                    const std::vector<std::string> &hierarchy,
                                    DataMap &returnValues)
{
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             values,
             hierarchy,
             returnValues);
    }

    return;
}

/**
 * @brief SakuraThread::processParallelPart
 */
void
SakuraThread::processParallelPart(ParallelBranching* growPlan,
                                  DataMap &values,
                                  const std::vector<std::string> &hierarchy,
                                  DataMap &returnValues)
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
