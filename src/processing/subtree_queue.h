#ifndef SUBTREE_QUEUE_H
#define SUBTREE_QUEUE_H

#include <common.h>
#include <items/sakura_items.h>

namespace SakuraTree
{

class SubtreeQueue
{
public:
    SubtreeQueue();

    struct ActiveCounter {
        std::mutex lock;
        uint32_t isCounter = 0;
        uint32_t shouldCount = 0;

        ActiveCounter() {}

        void increaseCounter()
        {
            lock.lock();
            isCounter++;
            lock.unlock();
        }

        bool isEqual()
        {
            bool result = false;
            lock.lock();
            result = isCounter == shouldCount;
            lock.unlock();
            return result;
        }
    };

    struct SubtreeObject
    {
        SakuraItem* subtree = nullptr;
        DataMap items;
        ActiveCounter* activeCounter = nullptr;
        std::vector<std::string> hirarchy;
    };

    bool addSubtree(const SubtreeObject &subtree);
    SubtreeObject getSubtree();

private:
    std::mutex m_lock;
    std::queue<SubtreeObject> m_queue;

};

}

#endif // SUBTREE_QUEUE_H
