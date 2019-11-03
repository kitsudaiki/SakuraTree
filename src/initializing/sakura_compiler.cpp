/**
 *  @file    sakura_compiler.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "sakura_compiler.h"
#include <libKitsunemimiSakuraParser/sakura_converter.h>
#include <initializing/file_collector.h>
#include <items/sakura_items.h>
#include <items/item_methods.h>

namespace SakuraTree
{

/**
 * @brief SakuraCompiler::SakuraCompiler
 * @param driver
 */
SakuraCompiler::SakuraCompiler(Kitsunemimi::Sakura::SakuraConverter* driver)
{
    m_driver = driver;
    m_fileCollector = new FileCollector(m_driver);
}

SakuraCompiler::~SakuraCompiler()
{
    delete m_fileCollector;
}

/**
 * @brief SakuraCompiler::compile
 * @param rootPath
 * @param seedName
 * @return
 */
SakuraItem*
SakuraCompiler::compile(const std::string &rootPath,
                        std::string &seedName)
{
    if(m_fileCollector->initFileCollector(rootPath) == false)
    {
        //TODO: replace with better solution
        std::cout<<"ERROR: "<<m_fileCollector->getErrorMessage()<<std::endl;
        exit(1);
    }

    if(seedName == "") {
        seedName = m_fileCollector->getSeedName(0);
    }

    DataMap* completePlan = m_fileCollector->getObject(seedName);
    assert(completePlan != nullptr);

    preProcessObject(completePlan);

    // debug-output
    if(DEBUG)
    {
        std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        std::string output = completePlan->toString(true);
        std::cout<<output<<std::endl;
        std::cout<<"-----------------------------------------------------"<<std::endl;
    }

    SakuraItem* result = convert(completePlan);

    return result;
}

/**
 * @brief SakuraCompiler::processObject
 * @param value
 * @return
 */
void
SakuraCompiler::preProcessObject(DataMap* value)
{
    // precheck
    if(value == nullptr) {
        return;
    }

    // end of tree
    if(value->get("type")->toString() == "blossom") {
        return;
    }

    // continue building
    DataMap* branch = value;

    if(value->get("type")->toString() == "tree"
            || value->get("type")->toString() == "branch")
    {
        branch = m_fileCollector->getObject(value->get("name")->toString());
        value->insert("parts", branch->get("parts"));
        value->insert("items", branch->get("items"));
    }

    if(value->contains("parts")) {
        preProcessArray(value->get("parts")->toArray());
    }
}

/**
 * @brief SakuraCompiler::processArray
 * @param value
 * @return
 */
void
SakuraCompiler::preProcessArray(DataArray* value)
{
    for(uint32_t i = 0; i < value->size(); i++)
    {
        preProcessObject(value->get(i)->toMap());
    }
}

/**
 * @brief SakuraCompiler::process
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convert(DataMap* growPlan)
{
    if(growPlan->getStringByKey("type") == "blossom") {
        return convertBlossom(growPlan);
    }

    if(growPlan->getStringByKey("type") == "branch") {
        return convertBranch(growPlan);
    }

    if(growPlan->getStringByKey("type") == "tree") {
        return convertTree(growPlan);
    }

    if(growPlan->getStringByKey("type") == "forest") {
        return convertForest(growPlan);
    }

    if(growPlan->getStringByKey("type") == "sequentiell") {
        return convertSequeniellPart(growPlan);
    }

    if(growPlan->getStringByKey("type") == "parallel") {
        return convertParallelPart(growPlan);
    }

    return nullptr;
}

/**
 * @brief SakuraCompiler::processBlossom
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertBlossom(DataMap* growPlan)
{
    BlossomItem* newItem =  new BlossomItem();
    newItem->name = growPlan->getStringByKey("name");
    newItem->settings = *dynamic_cast<DataMap*>(growPlan->get("common-settings"));
    newItem->blossomType = growPlan->getStringByKey("blossom-type");

    if(growPlan->contains("items-input"))
    {
        DataMap* itemsInput = dynamic_cast<DataMap*>(growPlan->get("items-input"));
        newItem->values = *itemsInput;
    }

    DataArray* subTypeArray = dynamic_cast<DataArray*>(growPlan->get("blossom-subtypes"));
    for(uint64_t i = 0; i < subTypeArray->size(); i++)
    {
        newItem->blossomSubTypes.push_back(subTypeArray->get(i)->toString());
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::convertBranch
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertBranch(DataMap* growPlan)
{
    BranchItem* newItem = new BranchItem();

    DataMap* items = dynamic_cast<DataMap*>(growPlan->get("items"));
    newItem->values = *items;

    if(growPlan->contains("items-input"))
    {
        DataMap* itemsInput = dynamic_cast<DataMap*>(growPlan->get("items-input"));
        overrideItems(newItem->values, *itemsInput);
    }

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));
        newItem->childs.push_back(convert(newMap));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::convertTree
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertTree(DataMap* growPlan)
{
    TreeItem* newItem = new TreeItem();

    DataMap* items = dynamic_cast<DataMap*>(growPlan->get("items"));
    newItem->values = *items;

    if(growPlan->contains("items-input"))
    {
        DataMap* itemsInput = dynamic_cast<DataMap*>(growPlan->get("items-input"));
        overrideItems(newItem->values, *itemsInput);
    }

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));
        newItem->childs.push_back(convert(newMap));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::convertForest
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertForest(DataMap* growPlan)
{
    ForestItem* newItem = new ForestItem();
    DataMap* items = dynamic_cast<DataMap*>(growPlan);

    newItem->name = items->get("name")->toValue()->getString();

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));

        TreeGroupItem* singlePart = new TreeGroupItem();
        singlePart->address = newMap->get("common-settings")->get("address")->toString();
        singlePart->port = newMap->get("common-settings")->get("port")->toValue()->getInt();
        singlePart->ssh_key = newMap->get("common-settings")->get("ssh_key")->toString();
        singlePart->user = newMap->get("common-settings")->get("user")->toString();

        singlePart->content = newMap->get("parts")->toString();

        newItem->childs.push_back(singlePart);
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::processSequeniellPart
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertSequeniellPart(DataMap* growPlan)
{
    SequeniellBranching* newItem = new SequeniellBranching();

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));
        newItem->childs.push_back(convert(newMap));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::processParallelPart
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertParallelPart(DataMap* growPlan)
{
    ParallelBranching* newItem = new ParallelBranching();

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));
        newItem->childs.push_back(convert(newMap));
    }

    return newItem;
}



}
