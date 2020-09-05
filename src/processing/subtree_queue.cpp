/**
 * @file        subtree_queue.cpp
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

#include "subtree_queue.h"

#include <processing/common/item_methods.h>

#include <libKitsunemimiPersistence/logger/logger.h>

/**
 * @brief constructor
 */
SubtreeQueue::SubtreeQueue() {}

/**
 * @brief add a new subtree-object to the queue
 *
 * @param newObject the new subtree-object, which should be added to the queue
 */
void
SubtreeQueue::addSubtreeObject(SubtreeObject* newObject)
{
    m_lock.lock();
    m_queue.push(newObject);
    m_lock.unlock();
}

/**
 * @brief SakuraRoot::spawnParallelSubtreesLoop
 * @param childs
 * @param startPos
 * @param endPos
 * @param filePath
 * @param hierarchy
 * @param parentValues
 * @param errorMessage
 * @return
 */
bool
SubtreeQueue::spawnParallelSubtreesLoop(SakuraItem* subtree,
                                        const uint64_t startPos,
                                        const uint64_t endPos,
                                        const std::string &filePath,
                                        const std::vector<std::string> &hierarchy,
                                        DataMap &parentValues,
                                        const std::string &tempVarName,
                                        DataArray* array,
                                        std::string &errorMessage)
{
    // create and initialize one counter-instance for all new subtrees
    ActiveCounter* activeCounter = new ActiveCounter();
    activeCounter->shouldCount = static_cast<uint32_t>(endPos - startPos);
    std::vector<SubtreeObject*> spawnedObjects;

    for(uint64_t i = startPos; i < endPos; i++)
    {
        // encapsulate the content of the loop together with the values and the counter-object
        // as an subtree-object and add it to the subtree-queue
        SubtreeObject* object = new SubtreeObject();
        object->subtree = subtree->copy();
        object->items = parentValues;
        object->hirarchy = hierarchy;
        object->activeCounter = activeCounter;
        object->filePath = filePath;

        // add the counter-variable as new value to be accessable within the loop
        if(array != nullptr) {
            object->items.insert(tempVarName, array->get(i)->copy(), true);
        } else {
            object->items.insert(tempVarName, new DataValue(static_cast<long>(i)), true);
        }

        addSubtreeObject(object);
        spawnedObjects.push_back(object);
    }

    bool result = waitUntilFinish(activeCounter, errorMessage);

    // post-processing and cleanup
    for(uint64_t i = startPos; i < endPos; i++)
    {
        std::string errorMessage = "";
        if(fillInputValueItemMap(subtree->values,
                                 spawnedObjects.at(static_cast<uint32_t>(i))->items,
                                 errorMessage) == false)
        {
            errorMessage = createError("subtree-processing",
                                       "error processing post-aggregation of for-loop:\n"
                                       + errorMessage);
            result = false;
        }
    }

    overrideItems(parentValues, subtree->values, ONLY_EXISTING);

    clearSpawnedObjects(spawnedObjects);

    return result;
}

/**
 * @brief SakuraRoot::spawnParallelSubtrees
 * @param childs
 * @param startPos
 * @param endPos
 * @param filePath
 * @param hierarchy
 * @param parentValues
 * @param errorMessage
 * @return
 */
bool
SubtreeQueue::spawnParallelSubtrees(const std::vector<SakuraItem*> &childs,
                                    const uint64_t startPos,
                                    const uint64_t endPos,
                                    const std::string &filePath,
                                    const std::vector<std::string> &hierarchy,
                                    const DataMap &parentValues,
                                    std::string &errorMessage,
                                    Kitsunemimi::Project::Session* session,
                                    const uint64_t blockerId)
{
    LOG_DEBUG("spawnParallelSubtrees");

    // TODO: check that startPos and endPos are not outside of the childs

    // create and initialize all threads
    ActiveCounter* activeCounter = new ActiveCounter();
    activeCounter->shouldCount = static_cast<uint32_t>(endPos - startPos);
    std::vector<SubtreeObject*> spawnedObjects;

    // encapsulate each subtree of the paralle part as subtree-object and add it to the
    // subtree-queue for parallel processing
    for(uint64_t i = startPos; i < endPos; i++)
    {
        SubtreeObject* object = new SubtreeObject();
        object->subtree = childs.at(i)->copy();
        object->hirarchy = hierarchy;
        object->items = parentValues;
        object->activeCounter = activeCounter;
        object->filePath = filePath;
        object->session = session;
        object->blockerId = blockerId;

        addSubtreeObject(object);
        spawnedObjects.push_back(object);
    }

    const bool result = waitUntilFinish(activeCounter, errorMessage);
    clearSpawnedObjects(spawnedObjects);

    return result;
}


/**
 * @brief getSubtreeObject take ta object from the queue and delete it from the queue
 *
 * @return first object in the queue or an empty-object, if nothing is in the queue
 */
SubtreeQueue::SubtreeObject*
SubtreeQueue::getSubtreeObject()
{
    SubtreeObject* subtree = nullptr;

    m_lock.lock();
    if(m_queue.empty() == false)
    {
        subtree = m_queue.front();
        m_queue.pop();
    }
    m_lock.unlock();

    return subtree;
}



/**
 * @brief waitUntilFinish
 * @param activeCounter
 * @param errorMessage
 * @return
 */
bool
SubtreeQueue::waitUntilFinish(ActiveCounter* activeCounter,
                              std::string &errorMessage)
{
    // wait until the created subtree was fully processed by the worker-threads
    while(activeCounter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    // in case of on error, forward this error to the upper layer
    const bool result = activeCounter->success;
    if(result == false) {
        errorMessage = activeCounter->outputMessage;
    }

    return result;
}

/**
 * @brief clearSpawnedObjects
 * @param spawnedObjects
 */
void
SubtreeQueue::clearSpawnedObjects(std::vector<SubtreeObject*> &spawnedObjects)
{
    ActiveCounter* activeCounter = nullptr;

    // free allocated resources
    for(SubtreeObject* obj : spawnedObjects)
    {
        activeCounter = obj->activeCounter;
        delete obj->subtree;
        delete obj;
    }

    if(activeCounter != nullptr) {
        delete activeCounter;
    }
}

