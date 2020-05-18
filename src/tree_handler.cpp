#include "tree_handler.h"

#include <sakura_root.h>
#include <items/sakura_items.h>
#include <converter/converter.h>

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
    m_converter = new Converter();
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
    const bool treeParseResult = m_parser->parseFiles(treePath);
    if(treeParseResult == false)
    {
        std::cout<<m_parser->getError().toString()<<std::endl;
        m_parser->m_idContentMapping.clear();
        return false;
    }

    // iterate over the result from the parser
    std::map<std::string, JsonItem>::iterator it;
    for(it = m_parser->m_idContentMapping.begin();
        it != m_parser->m_idContentMapping.end();
        it++)
    {
        // get values from map
        const std::string &treeId = it->first;
        const JsonItem plainTree = it->second;

        // check if tree is already in the map
        std::map<std::string, TreeHandlerItem>::const_iterator it;
        it = m_trees.find(treeId);
        if(it != m_trees.end())
        {
            LOG_ERROR("Tree-id already registered: " + treeId);
            m_parser->m_idContentMapping.clear();
            return false;
        }

        // parsing
        SakuraItem* processPlan = m_converter->convert(plainTree);
        if(processPlan == nullptr)
        {
            LOG_ERROR("Failed to convert tree: " + treeId);
            m_parser->m_idContentMapping.clear();
            return false;
        }

        // prepare TreeHandlerItem
        TreeHandlerItem newTreeHandlerItem;
        newTreeHandlerItem.convertedItem = processPlan;
        newTreeHandlerItem.parsedItem = plainTree;

        // add new item to the map
        m_trees.insert(std::make_pair(treeId, newTreeHandlerItem));
    }

    m_parser->m_idContentMapping.clear();

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
    JsonItem plainTree;
    std::string errorMessage = "";

    const bool parsingResult = plainTree.parse(content, errorMessage);
    if(parsingResult == false) {
        return false;
    }

    // check if tree is already in the map
    std::map<std::string, TreeHandlerItem>::const_iterator it;
    it = m_trees.find(treeId);
    if(it != m_trees.end()) {
        return false;
    }

    // parsing
    SakuraItem* processPlan = m_converter->convert(plainTree);
    if(processPlan == nullptr) {
        return false;
    }

    // prepare TreeHandlerItem
    TreeHandlerItem newTreeHandlerItem;
    newTreeHandlerItem.convertedItem = processPlan;
    newTreeHandlerItem.parsedItem = plainTree;

    // add new item to the map
    m_trees.insert(std::make_pair(treeId, newTreeHandlerItem));

    return true;
}

/**
 * @brief TreeHandler::getTree
 * @param treeId
 * @return
 */
SakuraItem*
TreeHandler::getConvertedTree(const std::string &treeId,
                              const std::string &initPath)
{
    if(treeId != "")
    {
        return getConvertedTreeFromMap(treeId);
    }
    else
    {
        if(m_trees.size() == 1)
        {
            return m_trees.begin()->second.convertedItem;
        }
        else if(Kitsunemimi::Persistence::isDir(initPath))
        {
            const bool treeParseResult = m_parser->parseFiles(initPath + "/root.tree");
            if(treeParseResult == false)
            {
                m_parser->m_idContentMapping.clear();
                return nullptr;
            }

            const JsonItem id = m_parser->m_idContentMapping.begin()->second.get("b_id");
            return getConvertedTreeFromMap(id.toString());
        }
    }

    return nullptr;
}

/**
 * @brief TreeHandler::getConvertedTreeFromMap
 * @param treeId
 * @return
 */
SakuraItem*
TreeHandler::getConvertedTreeFromMap(const std::string &treeId)
{
    std::map<std::string, TreeHandlerItem>::const_iterator it;

    // search in trees
    it = m_trees.find(treeId);
    if(it != m_trees.end()) {
        return it->second.convertedItem;
    }

    // search in predefined trees
    it = m_predefinedTrees.find(treeId);
    if(it != m_predefinedTrees.end()) {
        return it->second.convertedItem;
    }

    return nullptr;
}

/**
 * @brief TreeHandler::getParsedTree
 * @param treeId
 * @return
 */
const JsonItem
TreeHandler::getParsedTree(const std::string &treeId)
{
    std::map<std::string, TreeHandlerItem>::const_iterator it;
    it = m_trees.find(treeId);

    if(it != m_trees.end()) {
        return it->second.parsedItem;
    }

    return JsonItem();
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

    JsonItem parsedProvisioningSubtree;
    m_parser->parseString(parsedProvisioningSubtree, provisioningSubtree);
    if(parsedProvisioningSubtree.isValid() == false)
    {
        LOG_ERROR(m_parser->getError().toString());
        return false;
    }

    SakuraItem* convertedProvisioningSubtree = m_converter->convert(parsedProvisioningSubtree);

    // prepare TreeHandlerItem
    TreeHandlerItem newTreeHandlerItem;
    newTreeHandlerItem.convertedItem = convertedProvisioningSubtree;
    newTreeHandlerItem.parsedItem = parsedProvisioningSubtree;

    // add new item to the map
    m_predefinedTrees.insert(std::make_pair("sakura_provisioning", newTreeHandlerItem));

    return true;
}

}
