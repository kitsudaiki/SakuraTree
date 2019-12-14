#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <common.h>
#include <libKitsunemimiCommon/threading/thread.h>
#include <sakura_root.h>
#include <processing/subtree_queue.h>

namespace SakuraTree
{
class SakuraThread;

class ThreadPool
{
public:
    ThreadPool(const uint32_t numberOfThreads);
    ~ThreadPool();

    SubtreeQueue m_queue;
private:
    void clearChildThreads();

    std::vector<SakuraThread*> m_childThreads;
};

}

#endif // THREAD_POOL_H
