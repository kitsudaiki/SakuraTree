#include "tree_handler.h"

#include <sakura_root.h>

#include <libKitsunemimiSakuraParser/sakura_parsing.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/file_methods.h>

#include <sakura_provisioning_subtree.h>

namespace SakuraTree
{

/**
 * @brief TreeHandler::TreeHandler
 */
TreeHandler::TreeHandler()
{
    m_parser = new Kitsunemimi::Sakura::SakuraParsing(SakuraRoot::m_root->m_enableDebug);
}

/**
 * @brief parse and add new tree
 *
 * @param treePath absolut file-path
 * @param errorMessage reference for error-message
 *
 * @return true, if successful, else false
 */
bool
TreeHandler::addTree(const std::string &treePath,
                     std::string &errorMessage)
{
    // parse all files and convert the into
    const bool treeParseResult = m_parser->parseFiles(m_garden, treePath, errorMessage);
    if(treeParseResult == false) {
        return false;
    }

    return true;
}

/**
 * @brief convert path, which is relative to a tree-file, into a path, which is relative to the
 *        root-path.
 *
 * @param blossomFilePath absolut path of the file of the blossom
 * @param blossomInternalRelPath relative path, which is called inside of the blossom
 * @param addition directory addition ("templates" or "files")
 *
 * @return path, which is relative to the root-path.
 */
const std::string
TreeHandler::getRelativePath(const std::string &blossomFilePath,
                             const std::string &blossomInternalRelPath,
                             const std::string &addition)
{
    // create source-path
    const std::string parentPath = Kitsunemimi::Persistence::getParent(blossomFilePath);
    const std::string relativePath = Kitsunemimi::Persistence::getRelativePath(parentPath,
                                                                               m_garden.rootPath);

    // build new relative path for the new file-request
    if(relativePath == ".")
    {
        if(addition == "") {
            return blossomInternalRelPath;
        } else {
            return addition + "/" + blossomInternalRelPath;
        }
    }
    else
    {
        if(addition == "") {
            return relativePath + "/" + blossomInternalRelPath;;
        } else {
            return relativePath + "/" + addition + "/" + blossomInternalRelPath;
        }
    }
}

/**
 * @brief get a tree by its id
 *
 * @param id id of the requested tree
 *
 * @return requested pointer to tree-item
 */
TreeItem*
TreeHandler::getTreeById(const std::string &id)
{
    // get normal tree
    TreeItem* tree = m_garden.getTreeById(id);

    // if no tree with the id does not exist, check the predefined trees
    if(tree == nullptr)
    {
        std::map<std::string, TreeItem*>::const_iterator it;
        it = m_predefinedTrees.find(id);

        if(it != m_predefinedTrees.end()) {
            return it->second;
        }
    }

    return tree;
}

/**
 * @brief get a fully parsed tree
 *
 * @param relativePath relative path of the tree (Default: empty string)
 * @param rootPath root-path of the requested tree
 *
 * @return requested pointer to tree-item
 */
TreeItem*
TreeHandler::getTree(const std::string &relativePath,
                     const std::string &rootPath)
{
    // build complete file-path
    std::string completePath = relativePath;
    if(rootPath == "") {
        completePath = rootPath + "/" + relativePath;
    }

    // get tree-item based on the path
    if(Kitsunemimi::Persistence::isDir(completePath))
    {
        if(relativePath == "") {
            return m_garden.getTreeByPath("root.tree");
        } else {
            return m_garden.getTreeByPath(relativePath + "/root.tree");
        }
    }
    else
    {
        return m_garden.getTreeByPath(relativePath);
    }
}

/**
 * @brief load predefined subtrees
 */
bool
TreeHandler::loadPredefinedSubtrees()
{
    // get predifile provisioning subtree
    std::string provisioningSubtree(reinterpret_cast<char*>(sakura_provisioning_subtree_tree),
                                    sakura_provisioning_subtree_tree_len);
    Kitsunemimi::replaceSubstring(provisioningSubtree, "\\n", "\n");

    // parse string
    std::string errorMessage = "";
    SakuraItem* parsedItem = m_parser->parseString(provisioningSubtree, errorMessage);
    if(parsedItem == nullptr)
    {
        LOG_ERROR(errorMessage);
        return false;
    }

    // add new item to the map
    TreeItem* treeItem = static_cast<TreeItem*>(parsedItem);
    m_predefinedTrees.insert(std::make_pair("sakura_provisioning", treeItem));

    return true;
}

}
