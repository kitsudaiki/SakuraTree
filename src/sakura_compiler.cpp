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
#include <libKitsunemimiSakuraParser/file_collector.h>

#include <libKitsunemimiJson/json_item.h>

#include <sakura_root.h>
#include <items/sakura_items.h>
#include <items/item_methods.h>

#include <branch_builder/provision_branch_builder.h>

using Kitsunemimi::Json::JsonItem;

namespace SakuraTree
{

/**
 * @brief constructor
 */
SakuraCompiler::SakuraCompiler() {}

/**
 * @brief destructor
 */
SakuraCompiler::~SakuraCompiler() {}

/**
 * @brief SakuraCompiler::parseFiles
 * @param rootPath
 * @param seedName
 * @return
 */
JsonItem
SakuraCompiler::parseFiles(const std::string &rootPath,
                           std::string &seedName)
{
    SakuraParsing* sakuraParser = new SakuraParsing(DEBUG);
    FileCollector* fileCollector = new FileCollector(sakuraParser);

    if(fileCollector->initFileCollector(rootPath) == false)
    {
        //TODO: replace with better solution
        std::cout<<"ERROR: "<<fileCollector->getErrorMessage()<<std::endl;
        exit(1);
    }

    if(seedName == "") {
        seedName = fileCollector->getSeedName(0);
    }

    JsonItem completePlan = fileCollector->getObject(seedName);
    assert(completePlan.isValid());

    preProcessObject(completePlan, fileCollector);

    delete fileCollector;
    delete sakuraParser;

    return completePlan;
}

/**
 * @brief SakuraCompiler::compile
 * @param rootPath
 * @param seedName
 * @return
 */
SakuraItem*
SakuraCompiler::compile(JsonItem &tree)
{
    // debug-output
    if(DEBUG)
    {
        std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        const std::string output = tree.toString(true);
        std::cout<<output<<std::endl;
        std::cout<<"-----------------------------------------------------"<<std::endl;
    }

    SakuraItem* result = convert(tree);

    return result;
}

/**
 * @brief SakuraCompiler::processObject
 * @param value
 * @return
 */
void
SakuraCompiler::preProcessObject(JsonItem &object,
                                 FileCollector* fileCollector)
{
    // precheck
    if(object.isValid() == false) {
        return;
    }

    // end of tree
    if(object.get("btype").toString() == "blossom") {
        return;
    }

    // continue building
    JsonItem branch = object;

    if(object.get("btype").toString() == "tree"
            || object.get("btype").toString() == "branch")
    {
        branch = fileCollector->getObject(object.get("id").toString());
        object.insert("parts", branch.get("parts"));
        object.insert("items", branch.get("items"));
    }

    if(object.get("btype").toString() == "seed")
    {
        JsonItem subtree = object.get("subtree");
        preProcessObject(subtree, fileCollector);
    }

    if(object.contains("parts"))
    {
        JsonItem parts = object.get("parts");
        preProcessArray(parts, fileCollector);
    }

    if(object.contains("if_parts"))
    {
        JsonItem parts = object.get("if_parts");
        preProcessArray(parts, fileCollector);
    }

    if(object.contains("else_parts"))
    {
        JsonItem parts = object.get("else_parts");
        preProcessArray(parts, fileCollector);
    }
}

/**
 * @brief SakuraCompiler::processArray
 * @param value
 * @return
 */
void
SakuraCompiler::preProcessArray(JsonItem &object,
                                FileCollector* fileCollector)
{
    for(uint32_t i = 0; i < object.size(); i++)
    {
        JsonItem item = object.get(i);
        preProcessObject(item, fileCollector);
    }
}

/**
 * @brief SakuraCompiler::process
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convert(JsonItem &growPlan)
{
    const std::string typeName = growPlan.get("btype").toString();

    if(typeName == "blossom_group") {
        return convertBlossomGroup(growPlan);
    }

    if(typeName == "branch") {
        return convertBranch(growPlan);
    }

    if(typeName == "tree") {
        return convertTree(growPlan);
    }

    if(typeName == "sequentiell") {
        return convertSequeniellPart(growPlan);
    }

    if(typeName == "parallel") {
        return convertParallelPart(growPlan);
    }

    if(typeName == "seed") {
        return convertSeed(growPlan);
    }

    if(typeName == "if") {
        return convertIf(growPlan);
    }

    // it must everytime match one of the names
    assert(false);

    return nullptr;
}

/**
 * @brief SakuraCompiler::convertBlossom
 * @param growPlan
 * @return
 */
BlossomItem*
SakuraCompiler::convertBlossom(JsonItem &growPlan)
{
    BlossomItem* blossomItem =  new BlossomItem();

    blossomItem->blossomType = growPlan.get("blossom-type").toString();
    blossomItem->inputValues = growPlan.get("items-input").getItemContent()->copy()->toMap();

    const std::string outKey = growPlan.get("output").getItemContent()->toValue()->toString();
    blossomItem->outputValue = new DataMap();
    if(outKey.size() > 0) {
        blossomItem->outputValue->insert(outKey, nullptr);
    }

    return blossomItem;
}

/**
 * @brief SakuraCompiler::convertBlossomGroup
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertBlossomGroup(JsonItem &growPlan)
{
    BlossomGroupItem* blossomGroupItem =  new BlossomGroupItem();
    blossomGroupItem->id = growPlan.get("name").toString();
    blossomGroupItem->blossomGroupType = growPlan.get("blossom-group-type").toString();

    const JsonItem subTypeArray = growPlan.get("blossoms");
    if(subTypeArray.size() > 0)
    {
        for(uint32_t i = 0; i < subTypeArray.size(); i++)
        {
            JsonItem item = subTypeArray.get(i);
            BlossomItem* blossomItem = convertBlossom(item);

            JsonItem itemInput = growPlan.get("items-input");
            overrideItems(*blossomItem->inputValues, itemInput);

            blossomGroupItem->blossoms.push_back(blossomItem);
        }
    }
    else
    {
        BlossomItem* blossomItem =  new BlossomItem();

        blossomGroupItem->blossomGroupType = "special";
        blossomItem->blossomType = growPlan.get("blossom-group-type").toString();
        blossomItem->inputValues = growPlan.get("items-input").getItemContent()->copy()->toMap();
        blossomItem->outputValue = new DataMap();

        blossomGroupItem->blossoms.push_back(blossomItem);
    }

    return blossomGroupItem;
}

/**
 * @brief SakuraCompiler::convertBranch
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertBranch(JsonItem &growPlan)
{
    BranchItem* branchItem = new BranchItem();

    JsonItem items = growPlan.get("items");
    branchItem->id = growPlan.get("id").toString();
    branchItem->values = growPlan.get("items").getItemContent()->copy()->toMap();

    if(growPlan.contains("items-input"))
    {
        JsonItem itemInput = growPlan.get("items-input");
        overrideItems(*branchItem->values, itemInput);
    }

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
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
SakuraCompiler::convertTree(JsonItem &growPlan)
{
    TreeItem* treeItem = new TreeItem();

    treeItem->id = growPlan.get("id").toString();
    treeItem->values = growPlan.get("items").getItemContent()->copy()->toMap();

    if(growPlan.contains("items-input"))
    {
        JsonItem itemInput = growPlan.get("items-input");
        overrideItems(*treeItem->values, itemInput);
    }

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
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
SakuraCompiler::convertSeed(JsonItem &growPlan)
{
    SeedItem* seedItem = new SeedItem();

    seedItem->name = growPlan.get("id").toString();
    JsonItem connectionInfos = growPlan.get("connection");

    BranchItem* provisioningBranch = createProvisionBranch
            (
                connectionInfos.get("address").getString(),
                connectionInfos.get("ssh_port").getInt(),
                connectionInfos.get("ssh_user").getString(),
                connectionInfos.get("ssh_key").getString(),
                SakuraRoot::m_executablePath,
                "",
                growPlan.get("subtree").getString()
            );

    seedItem->child = provisioningBranch;

    return seedItem;
}

/**
 * @brief SakuraCompiler::convertIf
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertIf(JsonItem &growPlan)
{
    IfBranching* newItem = new IfBranching();

    if(growPlan.get("if_type").getString() == "==") {
        newItem->ifType = IfBranching::EQUAL;
    }
    if(growPlan.get("if_type").getString() == ">=") {
        newItem->ifType = IfBranching::GREATER_EQUAL;
    }
    if(growPlan.get("if_type").getString() == ">") {
        newItem->ifType = IfBranching::GREATER;
    }
    if(growPlan.get("if_type").getString() == "<=") {
        newItem->ifType = IfBranching::SMALLER_EQUAL;
    }
    if(growPlan.get("if_type").getString() == "<") {
        newItem->ifType = IfBranching::SMALLER;
    }
    if(growPlan.get("if_type").getString() == "!=") {
        newItem->ifType = IfBranching::UNEQUAL;
    }

    newItem->leftSide = growPlan.get("left").getItemContent()->copy()->toArray();
    newItem->rightSide = growPlan.get("right").getItemContent()->copy()->toValue();

    JsonItem if_parts = growPlan.get("if_parts");
    assert(if_parts.isValid());
    for(uint32_t i = 0; i < if_parts.size(); i++)
    {
        JsonItem newMap = if_parts.get(i);
        newItem->ifChilds.push_back(convert(newMap));
    }

    JsonItem else_parts = growPlan.get("else_parts");
    assert(else_parts.isValid());
    for(uint32_t i = 0; i < else_parts.size(); i++)
    {
        JsonItem newMap = else_parts.get(i);
        newItem->elseChilds.push_back(convert(newMap));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::processSequeniellPart
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertSequeniellPart(JsonItem &growPlan)
{
    SequeniellBranching* newItem = new SequeniellBranching();

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
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
SakuraCompiler::convertParallelPart(JsonItem &growPlan)
{
    ParallelBranching* newItem = new ParallelBranching();

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newItem->childs.push_back(convert(newMap));
    }

    return newItem;
}

}
