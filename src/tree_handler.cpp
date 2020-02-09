#include "tree_handler.h"

#include <items/sakura_items.h>
#include <libKitsunemimiSakuraParser/sakura_parsing.h>
#include <libKitsunemimiJson/json_item.h>
#include <converter/converter.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

#include <sakura_provisioning_subtree.h>

namespace SakuraTree
{

/**
 * @brief TreeHandler::TreeHandler
 */
TreeHandler::TreeHandler()
{
    m_converter = new Converter();
    m_parser = new Kitsunemimi::Sakura::SakuraParsing(DEBUG);
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
        return false;
    }

    for(uint32_t i = 0; i < m_parser->m_fileContents.size(); i++)
    {
        const std::string &treeId = m_parser->m_fileContents.at(i).first;
        const JsonItem plainTree = m_parser->m_fileContents.at(i).second;

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
        m_trees.insert(std::pair<std::string, TreeHandlerItem>(treeId, newTreeHandlerItem));
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
    m_trees.insert(std::pair<std::string, TreeHandlerItem>(treeId, newTreeHandlerItem));

    return true;
}

/**
 * @brief TreeHandler::getTree
 * @param treeId
 * @return
 */
SakuraItem*
TreeHandler::getConvertedTree(const std::string &treeId)
{
    std::map<std::string, TreeHandlerItem>::const_iterator it;
    it = m_trees.find(treeId);

    if(it != m_trees.end()) {
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
        std::cout<<m_parser->getError().toString()<<std::endl;
        return false;
    }

    SakuraItem* convertedProvisioningSubtree = m_converter->convert(parsedProvisioningSubtree);

    // prepare TreeHandlerItem
    TreeHandlerItem newTreeHandlerItem;
    newTreeHandlerItem.convertedItem = convertedProvisioningSubtree;
    newTreeHandlerItem.parsedItem = parsedProvisioningSubtree;

    // add new item to the map
    m_trees.insert(std::pair<std::string, TreeHandlerItem>(
                       "sakura_provisioning",
                       newTreeHandlerItem)
                   );

    return true;
}

}
