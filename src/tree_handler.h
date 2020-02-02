#ifndef TREE_HANDLER_H
#define TREE_HANDLER_H

#include <common.h>

namespace Kitsunemimi
{
namespace Sakura {
class SakuraParsing;
}
}

namespace SakuraTree
{
class SakuraItem;
class Converter;

class TreeHandler
{
public:
    TreeHandler();

    bool addTree(const std::string &treePath);
    bool addTree(const std::string &treeId,
                 SakuraItem* tree);
    SakuraItem* getTree(const std::string &treeId);

    bool loadPredefinedSubtrees();
private:
    std::map<std::string, SakuraItem*> m_trees;
    Converter* m_converter = nullptr;
    Kitsunemimi::Sakura::SakuraParsing* m_parser = nullptr;
};

}

#endif // TREE_HANDLER_H
