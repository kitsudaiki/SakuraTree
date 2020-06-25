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

#include <libKitsunemimiSakuraNetwork/sakura_network.h>

#include <processing/common/item_methods.h>
#include <processing/blossoms/blossom.h>
#include <processing/blossoms/blossom_getter.h>
#include <processing/subtree_queue.h>

#include <libKitsunemimiJinja2/jinja2_converter.h>
#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

namespace SakuraTree
{

/**
 * @brief constructor
 *
 * @param queue pointer to the subtree-queue, where new separated subtrees should be added
 */
SakuraThread::SakuraThread(SubtreeQueue* queue)
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
 * @brief run the main-loop of the thread and process each subtree, which can be taken from the
 *        queue
 */
void
SakuraThread::run()
{
    m_started = true;
    while(m_abort == false)
    {
        SubtreeQueue::SubtreeObject* currentSubtree = m_queue->getSubtreeObject();

        if(currentSubtree != nullptr)
        {
            if(currentSubtree->subtree != nullptr)
            {
                if(currentSubtree->session != nullptr) {
                    LOG_DEBUG("start Subcall");
                }

                // process input-values
                m_hierarchy = currentSubtree->hirarchy;
                overrideItems(m_parentValues, currentSubtree->subtree->values, false);
                overrideItems(m_parentValues, currentSubtree->items, false);

                // run the real task
                std::string errorMessage = "";
                const bool result = processSakuraItem(currentSubtree->subtree,
                                                      currentSubtree->filePath,
                                                      errorMessage);
                // handle result
                if(result) {
                    overrideItems(currentSubtree->items, m_parentValues, true);
                } else {
                    currentSubtree->activeCounter->registerError(errorMessage);
                }

                // increase active-counter as last step, so the source subtree can check, if all
                // spawned subtrees are finished
                currentSubtree->activeCounter->increaseCounter();

                if(currentSubtree->session != nullptr)
                {
                    char response[50];
                    LOG_DEBUG("FINISH Subcall");
                    currentSubtree->session->sendResponse(response, 50, currentSubtree->blockerId);
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(chronoMilliSec(10));
        }
    }
}

/**
 * @brief central method of the thread to process the current part of the execution-tree
 *
 * @param sakuraItem subtree, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processSakuraItem(SakuraItem* sakuraItem,
                                const std::string &filePath,
                                std::string &errorMessage)
{
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SEQUENTIELL_ITEM)
    {
        SequentiellPart* sequential = dynamic_cast<SequentiellPart*>(sakuraItem);
        return processSequeniellPart(sequential, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::TREE_ITEM)
    {
        TreeItem* subtreeItem = dynamic_cast<TreeItem*>(sakuraItem);
        m_hierarchy.push_back("TREE: " + subtreeItem->id);
        return processTree(subtreeItem, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SUBTREE_ITEM)
    {
        SubtreeItem* subtreeItem = dynamic_cast<SubtreeItem*>(sakuraItem);
        return processSubtree(subtreeItem, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::BLOSSOM_ITEM)
    {
        BlossomItem* blossomItem = dynamic_cast<BlossomItem*>(sakuraItem);
        return processBlossom(*blossomItem, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::BLOSSOM_GROUP_ITEM)
    {
        BlossomGroupItem* blossomGroupItem = dynamic_cast<BlossomGroupItem*>(sakuraItem);
        return processBlossomGroup(*blossomGroupItem, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::IF_ITEM)
    {
        IfBranching* ifBranching = dynamic_cast<IfBranching*>(sakuraItem);
        return processIf(ifBranching, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::FOR_EACH_ITEM)
    {
        ForEachBranching* forEachBranching = dynamic_cast<ForEachBranching*>(sakuraItem);
        return processForEach(forEachBranching, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::FOR_ITEM)
    {
        ForBranching* forBranching = dynamic_cast<ForBranching*>(sakuraItem);
        return processFor(forBranching, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::PARALLEL_ITEM)
    {
        ParallelPart* parallel = dynamic_cast<ParallelPart*>(sakuraItem);
        return processParallelPart(parallel, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SEED_ITEM)
    {
        SeedInitItem* seedItem = dynamic_cast<SeedInitItem*>(sakuraItem);
        return processSeedInit(seedItem, filePath, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SEED_TRIGGER_ITEM)
    {
        SeedTriggerItem* seedItem = dynamic_cast<SeedTriggerItem*>(sakuraItem);
        return processSeedTrigger(seedItem, errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    // TODO: error-message

    return false;
}

/**
 * @brief process single blossom
 *
 * @param blossomItem item with all information for the blossom
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processBlossom(BlossomItem &blossomItem,
                             const std::string &filePath,
                             std::string &errorMessage)
{
    LOG_DEBUG("processBlossom: \n");
    LOG_DEBUG("    name: " + blossomItem.blossomName);

    if(blossomItem.parentValues != nullptr) {
        LOG_DEBUG("    parentValues:" + blossomItem.parentValues->toString());
    }

    // process values by filling with information of the parent-object
    const bool result = fillInputValueItemMap(blossomItem.values, m_parentValues, errorMessage);
    LOG_DEBUG("    values:\n" + blossomItem.values.toString());

    if(result == false)
    {
        errorMessage = createError(blossomItem,
                                   "processing",
                                   "error while processing blossom items:\n    "
                                   + errorMessage);
        blossomItem.success = false;

        return false;
    }

    // get and prcess the requested blossom
    Blossom* blossom = getBlossom(blossomItem.blossomGroupType, blossomItem.blossomType);
    if(blossom == nullptr)
    {
        errorMessage = createError(blossomItem, "processing", "unknow blossom-type");
        blossomItem.success = false;
        return blossomItem.success;
    }

    blossomItem.parentValues = &m_parentValues;
    blossomItem.blossomPath = filePath;

    blossom->growBlossom(blossomItem, errorMessage);
    delete blossom;

    if(blossomItem.success == false) {
        return false;
    }

    // send result to root
    SakuraRoot::m_root->printOutput(blossomItem);

    // write processing result back to parent
    fillOutputValueItemMap(blossomItem.values, blossomItem.blossomOutput);

    // TODO: override only with the output-values to avoid unnecessary conflicts
    overrideItems(m_parentValues, blossomItem.values);

    return true;
}

/**
 * @brief process a group of blossoms
 *
 * @param blossomGroupItem object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processBlossomGroup(BlossomGroupItem &blossomGroupItem,
                                  const std::string &filePath,
                                  std::string &errorMessage)
{
    LOG_DEBUG("processBlossomGroup");

    // convert name as jinja2-string
    std::string convertResult = "";
    const bool ret = SakuraRoot::m_jinja2Converter->convert(convertResult,
                                                            blossomGroupItem.id,
                                                            &m_parentValues,
                                                            errorMessage);
    if(ret == false)
    {
        errorMessage = createError("jinja2-converter", errorMessage);
        return false;
    }

    // print blossom-group
    blossomGroupItem.nameHirarchie = m_hierarchy;
    blossomGroupItem.nameHirarchie.push_back("BLOSSOM-GROUP: " + blossomGroupItem.id);
    SakuraRoot::m_root->printOutput(blossomGroupItem);

    // iterate over all blossoms of the group and process one after another
    for(BlossomItem* blossomItem : blossomGroupItem.blossoms)
    {
        // update blossom-item with group-values for console-output
        blossomItem->blossomGroupType = blossomGroupItem.blossomGroupType;
        blossomItem->nameHirarchie = m_hierarchy;
        blossomItem->blossomName = blossomGroupItem.id;
        blossomItem->blossomGroupType = blossomGroupItem.blossomGroupType;
        blossomItem->nameHirarchie.push_back("BLOSSOM: " + convertResult);

        // copy values of the blossom-group into the blossom, but only values, which are not defined
        // which in the blossom
        overrideItems(blossomItem->values,
                      blossomGroupItem.values,
                      false,
                      true);

        if(processBlossom(*blossomItem, filePath, errorMessage) == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief process a new tree
 *
 * @param treeItem object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processTree(TreeItem* treeItem,
                          std::string &errorMessage)
{
    LOG_DEBUG("processTree");

    // check if there are uninitialized items in the tree
    const std::vector<std::string> uninitItems = checkItems(m_parentValues);
    if(uninitItems.size() > 0)
    {
        std::string message = "The following items are not initialized: \n";
        for(const std::string& uninitItem : uninitItems)
        {
            message += "    " + uninitItem + "\n";
        }
        errorMessage = createError("processing", message);

        return false;
    }

    // process items of the tree
    const std::string completePath = treeItem->rootPath + "/" + treeItem->relativePath;
    if(processSakuraItem(treeItem->childs, completePath, errorMessage) == false) {
        return false;
    }

    return true;
}

/**
 * @brief process a new subtree
 *
 * @param subtreeItem object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processSubtree(SubtreeItem* subtreeItem,
                             const std::string &filePath,
                             std::string &errorMessage)
{
    LOG_DEBUG("processSubtree");

    // get subtree-file based on the required path
    Kitsunemimi::Sakura::SakuraGarden* treeHandler = SakuraRoot::m_root->m_currentGarden;
    const bfs::path relPath = treeHandler->getRelativePath(filePath, subtreeItem->nameOrPath);
    SakuraItem* newSubtree = treeHandler->getTree(relPath, SakuraRoot::m_currentGarden->rootPath);
    if(newSubtree == nullptr)
    {
        errorMessage = createError("subtree-processing",
                                   "subtree doesn't exist: " + subtreeItem->nameOrPath);
        return false;
    }

    // fill normal map
    bool fillResult = fillInputValueItemMap(subtreeItem->values, m_parentValues, errorMessage);
    if(fillResult == false)
    {
        errorMessage = createError("subtree-processing",
                                   "error while processing blossom items:\n"
                                   + errorMessage);
        return false;
    }

    // pre-process subtree
    overrideItems(newSubtree->values, subtreeItem->values, false);

    // fill internal maps
    std::map<std::string, ValueItemMap>::iterator mapIt;
    for(mapIt = subtreeItem->internalSubtrees.begin();
        mapIt != subtreeItem->internalSubtrees.end();
        mapIt++)
    {
        DataMap* tempMap = new DataMap();

        std::map<std::string, ValueItem>::iterator valueIt;
        for(valueIt = mapIt->second.m_valueMap.begin();
            valueIt != mapIt->second.m_valueMap.end();
            valueIt++)
        {
            errorMessage = "";
            fillResult =  fillValueItem(valueIt->second, m_parentValues, errorMessage);
            if(fillResult == false)
            {
                errorMessage = createError("subtree-processing",
                                           "error while processing blossom items:\n"
                                           + errorMessage);
                return false;
            }

            tempMap->insert(valueIt->first, valueIt->second.item->copy());
        }

        newSubtree->values.insert(mapIt->first, tempMap);
    }

    // post-process subtree
    overrideItems(newSubtree->values, subtreeItem->values, false);
    overrideItems(m_parentValues, newSubtree->values, false);

    return processSakuraItem(newSubtree, filePath, errorMessage);
}

/**
 * @brief process a seed-init-item
 *
 * @param seedItem seed-init object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processSeedInit(SeedInitItem *seedItem,
                              const std::string &filePath,
                              std::string &)
{
    LOG_DEBUG("processSeedInit");

    // start server
    if(SakuraRoot::m_networking->createServer(SakuraRoot::m_serverPort) == false)
    {
        LOG_ERROR("failed to create server on port " + std::to_string(SakuraRoot::m_serverPort));
        return false;
    }

    // get predefined provisioning tree
    TreeItem* provisioningTree = SakuraRoot::m_currentGarden->getRessource("sakura_provisioning");
    assert(provisioningTree != nullptr);

    // prepare values for provisioning subtree
    DataMap values;
    values.insert("executable_path", new DataValue(SakuraRoot::m_executablePath), true);
    values.insert("server_port", new DataValue(SakuraRoot::m_serverPort), true);
    values.insert("server_ip_address", new DataValue(SakuraRoot::m_serverAddress), true);

    // create and initialize one counter-instance for all new subtrees
    SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
    counter->shouldCount = static_cast<uint32_t>(seedItem->childs.size());
    std::vector<SubtreeQueue::SubtreeObject*> spawnedObjects;

    // iterate of all hosts, which are defined within the seed-file
    for(SeedPart* part : seedItem->childs)
    {
        std::vector<std::string> tags;

        // get and convert tags inside the part
        DataArray* unconvertedTags = dynamic_cast<DataArray*>(part->values.get("tags"));
        if(unconvertedTags != nullptr)
        {
            for(uint32_t i = 0; i < unconvertedTags->size(); i++)
            {
                tags.push_back(unconvertedTags->get(i)->toString());
            }
        }

        // set host specific values
        values.insert("target_path", part->values.get("target_path"), true);
        values.insert("client_ip_address", part->values.get("ip_address"), true);
        values.insert("ssh_user", part->values.get("ssh_user"), true);
        values.insert("ssh_port", part->values.get("ssh_port"), true);
        values.insert("ssh_key_path", part->values.get("ssh_key_path"), true);

        // register host based on the information
        SakuraRoot::m_networking->registerHost(part->id, tags);

        // provision seed in separate thread
        SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
        object->subtree = provisioningTree->copy();
        object->items = values;
        object->hirarchy = m_hierarchy;
        object->activeCounter = counter;
        object->filePath = filePath;

        m_queue->addSubtreeObject(object);
        spawnedObjects.push_back(object);
    }

    // wait until the created subtree was fully processed by the worker-threads
    while(counter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    // wait until all hosts ready or until timeout after 10 seconds
    uint32_t maxTries = 100;
    // TODO: make timeout configurable
    while(maxTries > 0)
    {
        if(SakuraRoot::m_networking->areAllHostsReady() == true) {
            break;
        }

        usleep(100000);
        maxTries--;
    }

    // if timeout then fail
    if(maxTries == 0)
    {
        // TODO: better error
        LOG_ERROR("TIMEOUT");
        return false;
    }

    // send all sakura-files, templates and files too all hosts, which are defined within the seed
    SakuraRoot::m_networking->sendDataToAll(*SakuraRoot::m_currentGarden);

    return true;
}

/**
 * @brief process a seed-trigger-item
 *
 * @param seed-trigger object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processSeedTrigger(SeedTriggerItem* seedItem,
                                 std::string &errorMessage)
{
    LOG_DEBUG("processSeed");

    // fill normal map
    const bool fillResult = fillInputValueItemMap(seedItem->values, m_parentValues, errorMessage);
    if(fillResult == false)
    {
        errorMessage = createError("seed-processing",
                                   "error while processing blossom items:\n"
                                   + errorMessage);
        return false;
    }

    // prepare values
    DataMap fullValues = m_parentValues;
    overrideItems(fullValues, seedItem->values, false);

    SakuraRoot::m_networking->triggerSeedByTag(seedItem->tag,
                                               seedItem->treeId,
                                               fullValues.toString());

    return true;
}

/**
 * @brief process a if-else-condition
 *
 * @param ifCondition object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processIf(IfBranching* ifCondition,
                        const std::string &filePath,
                        std::string &errorMessage)
{
    LOG_DEBUG("processIf");

    // initialize
    bool ifMatch = false;
    ValueItem valueItem;

    // get left side of the comparism
    if(fillValueItem(ifCondition->leftSide, m_parentValues, errorMessage) == false)
    {
        errorMessage = createError("subtree-processing",
                                   "error processing if-condition:\n"
                                   + errorMessage);
        return false;
    }

    // get right side of the comparism
    if(fillValueItem(ifCondition->rightSide, m_parentValues, errorMessage) == false)
    {
        errorMessage = createError("subtree-processing",
                                   "error processing if-condition:\n"
                                   + errorMessage);
        return false;
    }

    const std::string leftSide = ifCondition->leftSide.item->toString();
    const std::string rightSide = ifCondition->rightSide.item->toString();

    // compare based on the compare-type
    switch(ifCondition->ifType)
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

    // based on the result, process the if-subtree or the else-subtree
    if(ifMatch) {
        return processSakuraItem(ifCondition->ifContent, filePath, errorMessage);
    } else {
        return processSakuraItem(ifCondition->elseContent, filePath, errorMessage);
    }
}

/**
 * @brief process a for-each-loop
 *
 * @param subtree object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processForEach(ForEachBranching* subtree,
                             const std::string &filePath,
                             std::string &errorMessage)
{
    LOG_DEBUG("processForEach");

    // initialize the array, over twhich the loop should iterate
    if(fillInputValueItemMap(subtree->iterateArray, m_parentValues, errorMessage) == false)
    {
        errorMessage = createError("subtree-processing",
                                   "error processing for-loop:\n"
                                   + errorMessage);
        return false;
    }

    DataArray* array = subtree->iterateArray.get("array")->toArray();

    // process content normal or parallel via worker-threads
    if(subtree->parallel == false)
    {
        // backup the parent-values to avoid permanent merging with loop-internal values
        DataMap preBalueBackup = m_parentValues;
        overrideItems(m_parentValues, subtree->values, false);

        for(uint32_t i = 0; i < array->size(); i++)
        {
            m_parentValues.insert(subtree->tempVarName, array->get(i), true);
            if(processSakuraItem(subtree->content->copy(), filePath, errorMessage) == false) {
                return false;
            }
        }

        // restore the old parent values and update only the existing values with the one form the
        // loop. That way, variables like the counter-variable are not added to the parent.
        DataMap postBalueBackup = m_parentValues;
        m_parentValues = preBalueBackup;
        overrideItems(m_parentValues, postBalueBackup);
    }
    else
    {
        // copy the parent-values
        DataMap internalValues = m_parentValues;

        // create and initialize one counter-instance for all new subtrees
        SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
        counter->shouldCount = static_cast<uint32_t>(array->size());
        std::vector<SubtreeQueue::SubtreeObject*> spawnedObjects;

        // iterate of all items of the array and for each item, create a subtree-object
        // and add it to the subtree-queue for parallel processing
        for(uint32_t i = 0; i < array->size(); i++)
        {
            internalValues.insert(subtree->tempVarName, array->get(i)->copy(), true);
            SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
            object->subtree = subtree->content->copy();
            object->items = internalValues;
            object->hirarchy = m_hierarchy;
            object->activeCounter = counter;
            object->filePath = filePath;

            m_queue->addSubtreeObject(object);
            spawnedObjects.push_back(object);
        }

        // wait until the created subtree was fully processed by the worker-threads
        while(counter->isEqual() == false) {
            std::this_thread::sleep_for(chronoMilliSec(10));
        }

        // in case of on error, forward this error to the upper layer
        if(counter->success == false)
        {
            errorMessage = counter->outputMessage;
            return false;
        }

        // post-processing and cleanup
        for(uint32_t i = 0; i < array->size(); i++)
        {
            std::string errorMessage = "";
            if(fillInputValueItemMap(subtree->values,
                                     spawnedObjects.at(static_cast<uint32_t>(i))->items,
                                     errorMessage) == false)
            {
                errorMessage = createError("subtree-processing",
                                           "error processing post-aggregation of for-loop:\n"
                                           + errorMessage);
                return false;
            }
        }

        overrideItems(m_parentValues, subtree->values, true);
    }

    return true;
}

/**
 * @brief process a for-loop
 *
 * @param subtree object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processFor(ForBranching* subtree,
                         const std::string &filePath,
                         std::string &errorMessage)
{
    LOG_DEBUG("processFor");

    // get start-value
    if(fillValueItem(subtree->start, m_parentValues, errorMessage) == false)
    {
        errorMessage = createError("subtree-processing",
                                   "error processing for-loop:\n"
                                   + errorMessage);
        return false;
    }

    const long startValue = subtree->start.item->toValue()->getLong();

    // get end-value
    if(fillValueItem(subtree->end, m_parentValues, errorMessage) == false)
    {
        errorMessage = createError("subtree-processing",
                                   "error processing for-loop:\n"
                                   + errorMessage);
        return false;
    }

    const long endValue = subtree->end.item->toValue()->getLong();

    // process content normal or parallel via worker-threads
    if(subtree->parallel == false)
    {
        // backup the parent-values to avoid permanent merging with loop-internal values
        DataMap preBalueBackup = m_parentValues;

        for(long i = startValue; i < endValue; i++)
        {
            // add the counter-variable as new value to be accessable within the loop
            m_parentValues.insert(subtree->tempVarName, new DataValue(i), true);

            const bool result = processSakuraItem(subtree->content, filePath, errorMessage);
            if(result == false) {
                return false;
            }
        }

        // restore the old parent values and update only the existing values with the one form the
        // loop. That way, variables like the counter-variable are not added to the parent.
        DataMap postBalueBackup = m_parentValues;
        m_parentValues = preBalueBackup;
        overrideItems(m_parentValues, postBalueBackup);
    }
    else
    {
        // copy the parent-values
        DataMap internalValues = m_parentValues;

        // create and initialize one counter-instance for all new subtrees
        SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
        counter->shouldCount = static_cast<uint32_t>(endValue - startValue);
        std::vector<SubtreeQueue::SubtreeObject*> spawnedObjects;

        for(long i = startValue; i < endValue; i++)
        {
            // add the counter-variable as new value to be accessable within the loop
            internalValues.insert(subtree->tempVarName, new DataValue(i), true);

            // encapsulate the content of the loop together with the values and the counter-object
            // as an subtree-object and add it to the subtree-queue
            SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
            object->subtree = subtree->content->copy();
            object->items = internalValues;
            object->hirarchy = m_hierarchy;
            object->activeCounter = counter;
            object->filePath = filePath;

            m_queue->addSubtreeObject(object);
            spawnedObjects.push_back(object);
        }

        // wait until the created subtree was fully processed by the worker-threads
        while(counter->isEqual() == false) {
            std::this_thread::sleep_for(chronoMilliSec(10));
        }

        // in case of on error, forward this error to the upper layer
        if(counter->success == false)
        {
            errorMessage = counter->outputMessage;
            return false;
        }

        // post-processing and cleanup
        for(long i = startValue; i < endValue; i++)
        {
            std::string errorMessage = "";
            if(fillInputValueItemMap(subtree->values,
                                     spawnedObjects.at(static_cast<uint32_t>(i))->items,
                                     errorMessage) == false)
            {
                errorMessage = createError("subtree-processing",
                                           "error processing post-aggregation of for-loop:\n"
                                           + errorMessage);
                return false;
            }
        }

        overrideItems(m_parentValues, subtree->values, true);
    }

    return true;
}

/**
 * @brief process sequentiall part
 *
 * @param subtree object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processSequeniellPart(SequentiellPart* subtree,
                                    const std::string &filePath,
                                    std::string &errorMessage)
{
    LOG_DEBUG("processSequeniellPart");

    for(SakuraItem* item : subtree->childs)
    {
        if(processSakuraItem(item, filePath, errorMessage) == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief process parallel part via worker-threads
 *
 * @param parallelPart object, which should be processed
 *
 * @return true if successful, else false
 */
bool
SakuraThread::processParallelPart(ParallelPart* parallelPart,
                                  const std::string &filePath,
                                  std::string &errorMessage)
{
    LOG_DEBUG("processParallelPart");

    SequentiellPart* parts = dynamic_cast<SequentiellPart*>(parallelPart->childs);

    // create and initialize all threads
    SubtreeQueue::ActiveCounter* counter = new SubtreeQueue::ActiveCounter();
    counter->shouldCount = static_cast<uint32_t>(parts->childs.size());
    std::vector<SubtreeQueue::SubtreeObject*> spawnedObjects;

    // encapsulate each subtree of the paralle part as subtree-object and add it to the
    // subtree-queue for parallel processing
    for(SakuraItem* item : parts->childs)
    {
        SubtreeQueue::SubtreeObject* object = new SubtreeQueue::SubtreeObject();
        object->subtree = item->copy();
        object->hirarchy = m_hierarchy;
        object->items = m_parentValues;
        object->activeCounter = counter;
        object->filePath = filePath;

        m_queue->addSubtreeObject(object);
        spawnedObjects.push_back(object);
    }

    // wait until the created subtree was fully processed by the worker-threads
    while(counter->isEqual() == false) {
        std::this_thread::sleep_for(chronoMilliSec(10));
    }

    // in case of on error, forward this error to the upper layer
    if(counter->success == false)
    {
        errorMessage = counter->outputMessage;
        return false;
    }

    return true;
}

}
