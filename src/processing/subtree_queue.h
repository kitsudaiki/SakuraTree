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

    struct SubtreeObject
    {
        SakuraItem* subtree = nullptr;
        DataMap items;
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
