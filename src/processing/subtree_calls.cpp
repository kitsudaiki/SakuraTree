#include "subtree_calls.h"

#include <sakura_root.h>

#include <processing/common/item_methods.h>
#include <processing/sakura_thread.h>
#include <processing/thread_pool.h>

#include <libKitsunemimiPersistence/logger/logger.h>

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
spawnParallelSubtreesLoop(SakuraItem* subtree,
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
    SubtreeQueue::ActiveCounter* activeCounter = new SubtreeQueue::ActiveCounter();
    activeCounter->shouldCount = static_cast<uint32_t>(endPos - startPos);
    std::vector<SubtreeQueue::SubtreeObject*> spawnedObjects;

    for(uint64_t i = startPos; i < endPos; i++)
    {
        // encapsulate the content of the loop together with the values and the counter-object
        // as an subtree-object and add it to the subtree-queue
        SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
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

        SakuraRoot::m_root->m_threadPool->m_queue.addSubtreeObject(object);
        spawnedObjects.push_back(object);
    }

    // wait until the created subtree was fully processed by the worker-threads
    while(activeCounter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    // in case of on error, forward this error to the upper layer
    bool result = activeCounter->success;
    if(result == false) {
        errorMessage = activeCounter->outputMessage;
    }

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

    // free allocated resources
    for(SubtreeQueue::SubtreeObject* obj : spawnedObjects)
    {
        delete obj->subtree;
        delete obj;
    }
    delete activeCounter;

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
spawnParallelSubtrees(const std::vector<SakuraItem*> &childs,
                      const uint64_t startPos,
                      const uint64_t endPos,
                      const std::string &filePath,
                      const std::vector<std::string> &hierarchy,
                      const DataMap &parentValues,
                      std::string &errorMessage)
{
    LOG_DEBUG("spawnParallelSubtrees");

    // TODO: check that startPos and endPos are not outside of the childs

    // create and initialize all threads
    SubtreeQueue::ActiveCounter* activeCounter = new SubtreeQueue::ActiveCounter();
    activeCounter->shouldCount = static_cast<uint32_t>(endPos - startPos);
    std::vector<SubtreeQueue::SubtreeObject*> spawnedObjects;

    // encapsulate each subtree of the paralle part as subtree-object and add it to the
    // subtree-queue for parallel processing
    for(uint64_t i = startPos; i < endPos; i++)
    {
        SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
        object->subtree = childs.at(i)->copy();
        object->hirarchy = hierarchy;
        object->items = parentValues;
        object->activeCounter = activeCounter;
        object->filePath = filePath;

        SakuraRoot::m_root->m_threadPool->m_queue.addSubtreeObject(object);
        spawnedObjects.push_back(object);
    }

    // wait until the created subtree was fully processed by the worker-threads
    while(activeCounter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    // in case of on error, forward this error to the upper layer
    const bool result = activeCounter->success;
    if(result == false) {
        errorMessage = activeCounter->outputMessage;
    }

    // free allocated resources
    for(SubtreeQueue::SubtreeObject* obj : spawnedObjects)
    {
        delete obj->subtree;
        delete obj;
    }
    delete activeCounter;

    return result;
}
