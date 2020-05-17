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

    struct TreeHandlerItem
    {
        JsonItem parsedItem;
        SakuraItem* convertedItem = nullptr;
    };

    TreeHandler();

    bool addTree(const std::string &treePath);
    bool addTree(const std::string &treeId,
                 const std::string &content);

    SakuraItem* getConvertedTree(const std::string &treeId);
    const JsonItem getParsedTree(const std::string &treeId);

    bool loadPredefinedSubtrees();

    std::map<std::string, TreeHandlerItem> m_trees;
    std::map<std::string, TreeHandlerItem> m_predefinedTrees;

private:
    Converter* m_converter = nullptr;
    Kitsunemimi::Sakura::SakuraParsing* m_parser = nullptr;
};

}

#endif // TREE_HANDLER_H
