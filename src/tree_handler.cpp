#include "tree_handler.h"

#include <items/sakura_items.h>
#include <libKitsunemimiSakuraParser/sakura_parsing.h>
#include <libKitsunemimiJson/json_item.h>
#include <converter/converter.h>

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
 * @param treeId
 * @param tree
 * @return
 */
bool
TreeHandler::addTree(const std::string &treeId,
                     const std::string &tree)
{
    // parse all files and convert the into
    const bool treeParseResult = m_parser->parseFiles(tree);
    if(treeParseResult == false)
    {
        std::cout<<m_parser->getError().toString()<<std::endl;
        return false;
    }

    // parsing
    JsonItem completePlan;
    std::string errorMessage = "";
    completePlan.parse(tree, errorMessage);

    SakuraItem* processPlan = m_converter->convert(completePlan);
    if(processPlan == nullptr) {
        return false;
    }

    return addTree(treeId, processPlan);
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

}
