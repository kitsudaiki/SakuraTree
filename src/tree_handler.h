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
class TreeHandler
{

public:
    TreeHandler();

    bool addTree(const std::string &treePath);
    bool addTree(const std::string &treeId,
                 const std::string &content);

    const std::string getRelativePath(const std::string &blossomFilePath,
                                      const std::string &blossomInternalRelPath,
                                      const std::string &addition = "");

    SakuraItem* getConvertedTree(const std::string &rootPath,
                                 const std::string &relativePath,
                                 const std::string &initialTreeId = "");
    const JsonItem getParsedTree(const std::string &treeId);

    bool loadPredefinedSubtrees();

    std::map<std::string, TreeItem*> m_predefinedTrees;
    Kitsunemimi::Sakura::SakuraGarden m_garden;

private:
    Kitsunemimi::Sakura::SakuraParsing* m_parser = nullptr;
};

}

#endif // TREE_HANDLER_H
