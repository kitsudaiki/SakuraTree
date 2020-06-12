/**
 * @file        thread_pool.cpp
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

#include "thread_pool.h"
#include <processing/sakura_thread.h>

namespace SakuraTree
{

/**
 * @brief constructor
 *
 * @param numberOfThreads number of initial created threads for the pool
 */
ThreadPool::ThreadPool(const uint32_t numberOfThreads)
{
    for(uint32_t i = 0; i < numberOfThreads; i++)
    {
        SakuraThread* child = new SakuraThread(&m_queue);
        m_childThreads.push_back(child);
        child->startThread();
    }
}

/**
 * @brief destructor
 */
ThreadPool::~ThreadPool()
{
    clearChildThreads();
}

/**
 * @brief stop and delete all threads of the pool
 */
void
ThreadPool::clearChildThreads()
{
    for(SakuraThread* childThread : m_childThreads)
    {
        childThread->stopThread();
        childThread->waitForFinish();
        delete childThread;
    }
    m_childThreads.clear();
}

}
