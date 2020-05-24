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
 * @brief TreeHandler::addTree
 * @param treePath
 * @return
 */
bool
TreeHandler::addTree(const std::string &treePath)
{
    // parse all files and convert the into
    std::string errorMessage = "";
    const bool treeParseResult = m_parser->parseFiles(m_garden, treePath, errorMessage);
    if(treeParseResult == false)
    {
        std::cout<<errorMessage<<std::endl;
        return false;
    }

    return true;
}

/**
 * @brief TreeHandler::addTree
 * @param treeId
 * @param content
 * @return
 */
bool
TreeHandler::addTree(const std::string &treeId,
                     const std::string &content)
{
    std::string errorMessage = "";
    const bool treeParseResult = m_parser->parseString(m_garden, treeId, content, errorMessage);
    if(treeParseResult == false)
    {
        std::cout<<errorMessage<<std::endl;
        return false;
    }

    return true;
}

/**
 * @brief TreeHandler::getTree
 * @param treeId
 * @return
 */
SakuraItem*
TreeHandler::getConvertedTree(const std::string &rootPath,
                              const std::string &relativePath,
                              const std::string &initialTreeId)
{
    if(initialTreeId != "")
    {
        return m_garden.getTreeById(initialTreeId);
    }
    else if(rootPath == "")
    {
        return m_garden.getTreeByPath(relativePath);
    }
    else
    {
        const std::string completePath = rootPath + "/" + relativePath;
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
}

/**
 * @brief TreeHandler::loadPredefinedSubtrees
 */
bool
TreeHandler::loadPredefinedSubtrees()
{
    std::string provisioningSubtree(reinterpret_cast<char*>(sakura_provisioning_subtree_tree),
                                    sakura_provisioning_subtree_tree_len);
    Kitsunemimi::replaceSubstring(provisioningSubtree, "\\n", "\n");

    std::string errorMessage = "";
    TreeItem* parsedItem = m_parser->parseString(provisioningSubtree, errorMessage);
    if(parsedItem == nullptr)
    {
        LOG_ERROR(errorMessage);
        return false;
    }

    // add new item to the map
    m_predefinedTrees.insert(std::make_pair("sakura_provisioning", parsedItem));

    return true;
}

}
