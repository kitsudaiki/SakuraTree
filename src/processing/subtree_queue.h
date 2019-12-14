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
        DataMap items = DataMap();
        std::vector<std::string> hirarchy;
    };

    bool addSubtree(const SubtreeObject &subtree);
    SubtreeObject getSubtree();

private:
    std::atomic_flag lock = ATOMIC_FLAG_INIT;
    std::queue<SubtreeObject> m_queue;

};

}

#endif // SUBTREE_QUEUE_H
