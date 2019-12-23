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

namespace SakuraTree
{

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
SubtreeQueue::addSubtreeObject(const SubtreeObject &newObject)
{
    m_lock.lock();
    m_queue.push(newObject);
    m_lock.unlock();
}

/**
 * @brief getSubtreeObject take ta object from the queue and delete it from the queue
 *
 * @return first object in the queue or an empty-object, if nothing is in the queue
 */
SubtreeQueue::SubtreeObject
SubtreeQueue::getSubtreeObject()
{
    SubtreeObject subtree;

    m_lock.lock();
    if(m_queue.empty() == false)
    {
        subtree = m_queue.front();
        m_queue.pop();
    }
    m_lock.unlock();

    return subtree;
}

}
