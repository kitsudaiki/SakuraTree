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

    loadPredefinedSubtrees();
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
        // parsing
        SakuraItem* processPlan = m_converter->convert(m_parser->m_fileContents.at(i).second);
        if(processPlan == nullptr) {
            return false;
        }

        if(addTree(m_parser->m_fileContents.at(i).first, processPlan) == false) {
            return false;
        }
    }

    return true;
}

/**
 * @brief TreeHandler::addTree
 * @param treeId
 * @param tree
 * @return
 */
bool
TreeHandler::addTree(const std::string &treeId,
                     SakuraItem* tree)
{
    std::map<std::string, SakuraItem*>::const_iterator it;
    it = m_trees.find(treeId);

    if(it != m_trees.end()) {
        return false;
    }

    m_trees.insert(std::pair<std::string, SakuraItem*>(treeId, tree));

    return true;
}

/**
 * @brief TreeHandler::getTree
 * @param treeId
 * @return
 */
SakuraItem*
TreeHandler::getTree(const std::string &treeId)
{
    std::map<std::string, SakuraItem*>::const_iterator it;
    it = m_trees.find(treeId);

    if(it != m_trees.end()) {
        return it->second;
    }

    return nullptr;
}

/**
 * @brief TreeHandler::loadPredefinedSubtrees
 */
void
TreeHandler::loadPredefinedSubtrees()
{
    std::string provisioningSubtree(reinterpret_cast<char*>(sakura_provisioning_subtree_tree),
                                    sakura_provisioning_subtree_tree_len);
    Kitsunemimi::replaceSubstring(provisioningSubtree, "\\n", "\n");

    JsonItem parsedProvisioningSubtree;
    m_parser->parseString(parsedProvisioningSubtree, provisioningSubtree);

    SakuraItem* convertedProvisioningSubtree = m_converter->convert(parsedProvisioningSubtree);

    addTree("sakura_provisioning", convertedProvisioningSubtree);
}

}
