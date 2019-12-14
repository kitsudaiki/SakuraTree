#include "subtree_queue.h"

namespace SakuraTree
{

SubtreeQueue::SubtreeQueue()
{

}

/**
 * @brief addSubtree
 * @param message
 * @return
 */
bool
SubtreeQueue::addSubtree(const SubtreeObject &subtree)
{
    bool result = false;
    while(lock.test_and_set(std::memory_order_acquire)) {
        ; // spin
    }
    m_queue.push(subtree);
    lock.clear(std::memory_order_release);
    return result;
}

/**
 * @brief getSubtree
 * @return
 */
SubtreeQueue::SubtreeObject
SubtreeQueue::getSubtree()
{
    SubtreeObject subtree;
    while(lock.test_and_set(std::memory_order_acquire)) {
        ; // spin
    }

    if(m_queue.empty() == false)
    {
        subtree = m_queue.front();
        m_queue.pop();
    }

    lock.clear(std::memory_order_release);

    return subtree;
}

}
