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
#include <processing/common/item_methods.h>
#include <processing/blossoms/blossom.h>
#include <processing/blossoms/blossom_getter.h>
#include <processing/subtree_queue.h>
#include <libKitsunemimiJinja2/jinja2_converter.h>

namespace SakuraTree
{

/**
 * constructor
 */
SakuraThread::SakuraThread(SubtreeQueue *queue)
{
    m_queue = queue;
}

/**
 * destructor
 */
SakuraThread::~SakuraThread()
{
}

/**
 * start thread
 */
void
SakuraThread::run()
{
    m_started = true;
    while(m_abort == false)
    {
        SubtreeQueue::SubtreeObject currentSubtree = m_queue->getSubtree();
        if(currentSubtree.subtree == nullptr)
        {
            std::this_thread::sleep_for(chronoMicroSec(10));
        }
        else
        {
            m_hierarchy = currentSubtree.hirarchy;
            overrideItems(m_parentValues, currentSubtree.subtree->values, false);
            overrideItems(m_parentValues, currentSubtree.items, false);
            grow(currentSubtree.subtree);
            if(currentSubtree.activeCounter != nullptr) {
                currentSubtree.activeCounter->increaseCounter();
            }
        }
    }
}

/**
 * central method of the thread to process the current part of the execution-tree
 */
bool
SakuraThread::grow(SakuraItem* subtree)
{
    if(subtree->getType() == SakuraItem::SUBTREE_ITEM)
    {
        SubtreeItem* subtreeItem = dynamic_cast<SubtreeItem*>(subtree);
        m_hierarchy.push_back("SUBTREE: " + subtreeItem->id);
        return processSubtree(subtreeItem);
    }

    if(subtree->getType() == SakuraItem::BLOSSOM_ITEM)
    {
        BlossomItem* blossomItem = dynamic_cast<BlossomItem*>(subtree);
        return processBlossom(*blossomItem);
    }

    if(subtree->getType() == SakuraItem::BLOSSOM_GROUP_ITEM)
    {
        BlossomGroupItem* blossomGroupItem = dynamic_cast<BlossomGroupItem*>(subtree);
        return processBlossomGroup(*blossomGroupItem);
    }

    if(subtree->getType() == SakuraItem::IF_ITEM)
    {
        IfBranching* ifBranching = dynamic_cast<IfBranching*>(subtree);
        return processIf(ifBranching);
    }

    if(subtree->getType() == SakuraItem::FOR_EACH_ITEM)
    {
        ForEachBranching* forEachBranching = dynamic_cast<ForEachBranching*>(subtree);
        return processForEach(forEachBranching, false);
    }

    if(subtree->getType() == SakuraItem::FOR_ITEM)
    {
        ForBranching* forBranching = dynamic_cast<ForBranching*>(subtree);
        return processFor(forBranching, false);
    }

    if(subtree->getType() == SakuraItem::PARALLEL_FOR_EACH_ITEM)
    {
        ForEachBranching* forEachBranching = dynamic_cast<ForEachBranching*>(subtree);
        return processForEach(forEachBranching, true);
    }

    if(subtree->getType() == SakuraItem::PARALLEL_FOR_ITEM)
    {
        ForBranching* forBranching = dynamic_cast<ForBranching*>(subtree);
        return processFor(forBranching, true);
    }

    if(subtree->getType() == SakuraItem::SEQUENTIELL_ITEM)
    {
        Sequentiell* sequentiell = dynamic_cast<Sequentiell*>(subtree);
        return processSequeniellPart(sequentiell);
    }

    if(subtree->getType() == SakuraItem::PARALLEL_ITEM)
    {
        Parallel* parallel = dynamic_cast<Parallel*>(subtree);
        return processParallelPart(parallel);
    }

    if(subtree->getType() == SakuraItem::SEED_ITEM)
    {
        SeedItem* forestItem = dynamic_cast<SeedItem*>(subtree);
        SubtreeItem* branchItem = dynamic_cast<SubtreeItem*>(forestItem->child);
        return processSubtree(branchItem);
    }

    return false;
}

/**
 * @brief SakuraThread::processBlossom
 */
bool
SakuraThread::processBlossom(BlossomItem &subtree)
{
    const bool result = fillInputValueItemMap(subtree.values, m_parentValues);
    if(result == false) {
        return false;
    }

    Blossom* blossom = getBlossom(subtree.blossomGroupType,
                                  subtree.blossomType);
    subtree.parentValues = &m_parentValues;
    blossom->growBlossom(subtree);
    delete blossom;

    // send result to root
    SakuraRoot::m_root->printOutput(subtree);

    // write processing result back to parent
    fillOutputValueItemMap(subtree.values, subtree.blossomOutput);

    overrideItems(m_parentValues, subtree.values);

    return subtree.success;
}

/**
 * @brief SakuraThread::processBlossomGroup
 * @param subtree
 * @param values
 * @param hierarchy
 */
bool
SakuraThread::processBlossomGroup(BlossomGroupItem &subtree)
{
    for(uint32_t i = 0; i < subtree.blossoms.size(); i++)
    {
        BlossomItem* blossomItem = subtree.blossoms.at(i);
        blossomItem->blossomGroupType = subtree.blossomGroupType;
        blossomItem->nameHirarchie = m_hierarchy;
        blossomItem->nameHirarchie.push_back("BLOSSOM: " + subtree.id);

        const bool result = grow(blossomItem);
        if(result == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief SakuraThread::processBranch
 */
bool
SakuraThread::processSubtree(SubtreeItem* subtree)
{
    for(uint32_t i = 0; i < subtree->childs.size(); i++)
    {
        const bool result = grow(subtree->childs.at(i));;
        if(result == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief SakuraThread::processIf
 * @param subtree
 * @param values
 * @param hierarchy
 */
bool
SakuraThread::processIf(IfBranching* subtree)
{
    bool ifMatch = false;
    ValueItem valueItem;

    if(fillValueItem(subtree->leftSide, m_parentValues) == false) {
        return false;
    }
    const std::string  leftSide = subtree->leftSide.item->toValue()->toString();

    if(fillValueItem(subtree->rightSide, m_parentValues) == false) {
        return false;
    }
    const std::string  rightSide = subtree->rightSide.item->toValue()->toString();

    switch(subtree->ifType)
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

    if(ifMatch) {
        return grow(subtree->ifContent);
    } else {
        return grow(subtree->elseContent);
    }
}

/**
 * @brief SakuraThread::processForEach
 * @param subtree
 * @param values
 * @param hierarchy
 */
bool
SakuraThread::processForEach(ForEachBranching* subtree,
                             bool parallel)
{
    DataMap preBalueBackup = m_parentValues;
    fillInputValueItemMap(subtree->iterateArray, m_parentValues);
    overrideItems(m_parentValues, subtree->values, false);

    if(parallel == false)
    {
        DataArray* array = subtree->iterateArray.get("array")->toArray();
        for(uint32_t i = 0; i < array->size(); i++)
        {
            m_parentValues.insert(subtree->tempVarName, array->get(i), true);
            const bool result = grow(subtree->content->copy());
            if(result == false) {
                return false;
            }
        }

    }
    else
    {
        // create and initialize all threads
        DataArray* array = subtree->iterateArray.get("array")->toArray();
        SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
        counter->shouldCount = static_cast<uint32_t>(array->size());

        for(uint32_t i = 0; i < array->size(); i++)
        {
            m_parentValues.insert(subtree->tempVarName, array->get(i)->copy(), true);
            SubtreeQueue::SubtreeObject object;
            object.subtree = subtree->content->copy();
            object.items = m_parentValues;
            object.hirarchy = m_hierarchy;
            object.activeCounter = counter;
            m_queue->addSubtree(object);
        }

        while(counter->isEqual() == false) {
            std::this_thread::sleep_for(chronoMilliSec(10));
        }
    }

    DataMap postBalueBackup = m_parentValues;
    m_parentValues = preBalueBackup;
    overrideItems(m_parentValues, postBalueBackup);

    return true;
}

/**
 * @brief SakuraThread::processFor
 * @param subtree
 * @param values
 * @param hierarchy
 */
bool
SakuraThread::processFor(ForBranching* subtree,
                         bool parallel)
{
    if(fillValueItem(subtree->start, m_parentValues) == false) {
        return false;
    }
    const long startValue = subtree->start.item->toValue()->getLong();

    if(fillValueItem(subtree->end, m_parentValues) == false) {
        return false;
    }
    const long endValue = subtree->end.item->toValue()->getLong();

    DataMap preBalueBackup = m_parentValues;
    overrideItems(m_parentValues, subtree->values, false);

    if(parallel == false)
    {
        for(long i = startValue; i < endValue; i++)
        {
            m_parentValues.insert(subtree->tempVarName, new DataValue(i), true);
            const bool result = grow(subtree->content);
            if(result == false) {
                return false;
            }
        }
    }
    else
    {
        SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
        counter->shouldCount = static_cast<uint32_t>(endValue - startValue);

        for(long i = startValue; i < endValue; i++)
        {
            m_parentValues.insert(subtree->tempVarName, new DataValue(i), true);
            SubtreeQueue::SubtreeObject object;
            object.subtree = subtree->content->copy();
            object.items = m_parentValues;
            object.hirarchy = m_hierarchy;
            object.activeCounter = counter;
            m_queue->addSubtree(object);
        }

        while(counter->isEqual() == false) {
            std::this_thread::sleep_for(chronoMilliSec(10));
        }
    }

    DataMap postBalueBackup = m_parentValues;
    m_parentValues = preBalueBackup;
    overrideItems(m_parentValues, postBalueBackup);

    return true;
}

/**
 * @brief SakuraThread::processSequeniellPart
 */
bool
SakuraThread::processSequeniellPart(Sequentiell* subtree)
{
    for(uint32_t i = 0; i < subtree->childs.size(); i++)
    {
        const bool result = grow(subtree->childs.at(i));;
        if(result == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief SakuraThread::processParallelPart
 */
bool
SakuraThread::processParallelPart(Parallel *subtree)
{
    // create and initialize all threads
    SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
    counter->shouldCount = static_cast<uint32_t>(subtree->childs.size());

    for(uint32_t i = 0; i < subtree->childs.size(); i++)
    {
        SubtreeQueue::SubtreeObject object;
        object.subtree = subtree->childs.at(i);
        object.hirarchy = m_hierarchy;
        object.items = m_parentValues;
        object.activeCounter = counter;
        m_queue->addSubtree(object);
    }

    while(counter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    return true;
}

}
