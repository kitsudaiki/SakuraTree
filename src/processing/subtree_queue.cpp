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
    m_lock.lock();

    m_queue.push(subtree);

    m_lock.unlock();
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
