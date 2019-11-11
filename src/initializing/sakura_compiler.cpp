/**
 * @file        sakura_compiler.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "sakura_compiler.h"

#include <libKitsunemimiSakuraParser/sakura_parsing.h>

#include <libKitsunemimiJson/json_item.h>

#include <initializing/file_collector.h>
#include <sakura_root.h>
#include <items/sakura_items.h>
#include <items/item_methods.h>

#include <branch_builder/provision_branch_builder.h>

using Kitsunemimi::Json::JsonItem;

namespace SakuraTree
{

/**
 * @brief SakuraCompiler::SakuraCompiler
 * @param driver
 */
SakuraCompiler::SakuraCompiler(Kitsunemimi::Sakura::SakuraParsing* driver)
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
 * @brief SakuraCompiler::compileSubtree
 * @param subtree
 * @return
 */
SakuraItem*
SakuraCompiler::compileSubtree(const std::string subtree)
{
    JsonItem json;
    json.parse(subtree);

    DataMap* completePlan = json.getItemContent()->copy()->toMap();

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
SakuraCompiler::preProcessObject(DataMap* object)
{
    // precheck
    if(object == nullptr) {
        return;
    }

    // end of tree
    if(object->get("btype")->toString() == "blossom") {
        return;
    }

    // continue building
    DataMap* branch = object;

    if(object->get("btype")->toString() == "tree"
            || object->get("btype")->toString() == "branch")
    {
        branch = m_fileCollector->getObject(object->get("id")->toString());
        object->insert("parts", branch->get("parts"));
        object->insert("items", branch->get("items"));
    }

    if(object->get("btype")->toString() == "seed")
    {
        branch = m_fileCollector->getObject(object->get("id")->toString());
        preProcessObject(object->get("subtree")->toMap());
    }

    if(object->contains("parts")) {
        preProcessArray(object->get("parts")->toArray());
    }
}

/**
 * @brief SakuraCompiler::processArray
 * @param value
 * @return
 */
void
SakuraCompiler::preProcessArray(DataArray* object)
{
    for(uint32_t i = 0; i < object->size(); i++)
    {
        preProcessObject(object->get(i)->toMap());
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
    if(growPlan->getStringByKey("btype") == "blossom") {
        return convertBlossom(growPlan);
    }

    if(growPlan->getStringByKey("btype") == "branch") {
        return convertBranch(growPlan);
    }

    if(growPlan->getStringByKey("btype") == "tree") {
        return convertTree(growPlan);
    }

    if(growPlan->getStringByKey("btype") == "sequentiell") {
        return convertSequeniellPart(growPlan);
    }

    if(growPlan->getStringByKey("btype") == "parallel") {
        return convertParallelPart(growPlan);
    }

    if(growPlan->getStringByKey("btype") == "seed") {
        return convertSeed(growPlan);
    }

    assert(false);

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
    BlossomItem* blossomItem =  new BlossomItem();
    blossomItem->id = growPlan->getStringByKey("name");
    blossomItem->blossomType = growPlan->getStringByKey("blossom-type");

    DataArray* subTypeArray = dynamic_cast<DataArray*>(growPlan->get("blossom-leafs"));
    for(uint64_t i = 0; i < subTypeArray->size(); i++)
    {
        if(subTypeArray->get(i)->toMap()->contains("items-input"))
        {
            DataMap* itemsInput = dynamic_cast<DataMap*>(
                        subTypeArray->get(i)->toMap()->get("items-input"));
            blossomItem->values = *itemsInput;
        }

        blossomItem->blossomSubTypes.push_back(
                    subTypeArray->get(i)->toMap()->get("blossom-subtype")->toString());
    }

    return blossomItem;
}

/**
 * @brief SakuraCompiler::convertBranch
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertBranch(DataMap* growPlan)
{
    BranchItem* branchItem = new BranchItem();

    DataMap* items = growPlan->get("items")->toMap();
    branchItem->values = *items;

    if(growPlan->contains("items-input"))
    {
        DataMap* itemsInput = dynamic_cast<DataMap*>(growPlan->get("items-input"));
        overrideItems(branchItem->values, *itemsInput);
    }

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));
        branchItem->childs.push_back(convert(newMap));
    }

    return branchItem;
}

/**
 * @brief SakuraCompiler::convertTree
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertTree(DataMap* growPlan)
{
    TreeItem* treeItem = new TreeItem();

    DataMap* items = dynamic_cast<DataMap*>(growPlan->get("items"));
    treeItem->values = *items;

    if(growPlan->contains("items-input"))
    {
        DataMap* itemsInput = dynamic_cast<DataMap*>(growPlan->get("items-input"));
        overrideItems(treeItem->values, *itemsInput);
    }

    DataItem* parts = growPlan->get("parts");
    assert(parts != nullptr);

    for(uint32_t i = 0; i < parts->size(); i++)
    {
        DataMap* newMap = dynamic_cast<DataMap*>(parts->get(i));
        treeItem->childs.push_back(convert(newMap));
    }

    return treeItem;
}

/**
 * @brief SakuraCompiler::convertForest
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertSeed(DataMap* growPlan)
{
    SeedItem* seedItem = new SeedItem();
    DataMap* items = dynamic_cast<DataMap*>(growPlan);

    seedItem->name = items->get("id")->toValue()->getString();

    BranchItem* provisioningBranch = createProvisionBranch
            (
                growPlan->get("connection")->get("address")->toString(),
                growPlan->get("connection")->get("ssh_port")->toValue()->getInt(),
                growPlan->get("connection")->get("ssh_user")->toString(),
                growPlan->get("connection")->get("ssh_key")->toString(),
                SakuraRoot::m_executablePath,
                "",
                growPlan->get("subtree")->toString()
            );

    seedItem->child = provisioningBranch;

    return seedItem;
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
