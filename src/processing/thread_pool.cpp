#include "thread_pool.h"
#include <processing/sakura_thread.h>

namespace SakuraTree
{

ThreadPool::ThreadPool(const uint32_t numberOfThreads)
{
    for(uint32_t i = 0; i < numberOfThreads; i++)
    {
        SakuraThread* child = new SakuraThread(&m_queue);
        m_childThreads.push_back(child);
        child->startThread();
    }

}

ThreadPool::~ThreadPool()
{
    clearChildThreads();
}

/**
 * stop and delete all child processes
 */
void
ThreadPool::clearChildThreads()
{
    for(uint32_t i = 0; i < m_childThreads.size(); i++)
    {
        m_childThreads.at(i)->stopThread();
        m_childThreads.at(i)->waitForFinish();
        delete m_childThreads[i];
    }
    m_childThreads.clear();
}

}
