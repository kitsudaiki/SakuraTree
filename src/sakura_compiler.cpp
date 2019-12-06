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

    SakuraItem* result = convert(tree, nullptr);

    return result;
}

/**
 * @brief SakuraCompiler::convertItemPart
 * @param itemInput
 * @param itemPart
 * @return
 */
bool
SakuraCompiler::convertItemPart(JsonItem &itemInput,
                                ValueItem &itemPart,
                                const std::string pairType)
{
    if(itemInput.isMap())
    {
        itemPart.item = itemInput.get("item").getItemContent()->copy()->toValue();
    }
    else if (itemInput.isArray())
    {
        DataArray* tempArray = new DataArray();
        for(uint32_t i = 0; i < itemInput.size(); i++)
        {
            tempArray->append(itemInput.get(i).getItemContent()->copy());
        }
        itemPart.item = tempArray;
    }

    if(pairType == "assign") {
        itemPart.type = ValueItem::INPUT_PAIR_TYPE;
    }
    if(pairType == "compare") {
        itemPart.type = ValueItem::COMPARE_EQUAL_PAIR_TYPE;
    }
    if(pairType == "output") {
        itemPart.type = ValueItem::OUTPUT_PAIR_TYPE;
    }

    if(itemInput.get("type").toString() == "identifier") {
        itemPart.isIdentifier = true;
    }

    JsonItem functions = itemInput.get("functions");
    for(uint32_t f = 0; f < functions.size(); f++)
    {
        FunctionItem functionItem;

        JsonItem arguments = functions.get("args");
        for(uint32_t a = 0; a < arguments.size(); a++)
        {
            DataValue* arg = arguments.get(a).getItemContent()->toValue();
            functionItem.arguments.push_back(*arg);
        }

        if(functions.get(f).get("m_type").toString() == "get") {
            functionItem.type = FunctionItem::GET_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "split") {
            functionItem.type = FunctionItem::SPLIT_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "contains") {
            functionItem.type = FunctionItem::CONTAINS_FUNCTION;
        }

        itemPart.functions.push_back(functionItem);
    }

    return true;
}

/**
 * @brief overrideItems
 */
void
SakuraCompiler::overrideItems(JsonItem &original,
                              JsonItem &override)
{
    const std::vector<std::string> keys = override.getKeys();
    for(uint32_t i = 0; i < keys.size(); i++)
    {
        original.insert(keys.at(i),
                        override.get(keys.at(i)).getItemContent()->copy(),
                        true);
    }
}

/**
 * @brief SakuraCompiler::process
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convert(JsonItem &growPlan,
                        SakuraItem* parent)
{
    const std::string typeName = growPlan.get("b_type").toString();

    if(typeName == "blossom_group") {
        return convertBlossomGroup(growPlan, parent);
    }

    if(typeName == "branch") {
        return convertBranch(growPlan, parent);
    }

    if(typeName == "tree") {
        return convertTree(growPlan, parent);
    }

    if(typeName == "sequentiell") {
        return convertSequeniellPart(growPlan, parent);
    }

    if(typeName == "parallel") {
        return convertParallelPart(growPlan, parent);
    }

    if(typeName == "seed") {
        return convertSeed(growPlan, parent);
    }

    if(typeName == "if") {
        return convertIf(growPlan, parent);
    }

    if(typeName == "for_each") {
        return convertForEach(growPlan, parent);
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
SakuraCompiler::convertBlossom(JsonItem &growPlan,
                               SakuraItem* parent)
{
    BlossomItem* blossomItem =  new BlossomItem();
    blossomItem->blossomType = growPlan.get("blossom-type").toString();
    blossomItem->parent = parent;

    JsonItem array = growPlan.get("items-input");
    for(uint32_t i = 0; i < array.size(); i++)
    {
        const std::string key = array.get(i).get("key").toString();
        const std::string pairType = array.get(i).get("type").toString();
        JsonItem value = array.get(i).get("value");

        ValueItem itemValue;
        convertItemPart(value, itemValue, pairType);
        blossomItem->values.insert(key, itemValue);
    }

    return blossomItem;
}

/**
 * @brief SakuraCompiler::convertBlossomGroup
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertBlossomGroup(JsonItem &growPlan,
                                    SakuraItem* parent)
{
    BlossomGroupItem* blossomGroupItem =  new BlossomGroupItem();
    blossomGroupItem->id = growPlan.get("name").toString();
    blossomGroupItem->blossomGroupType = growPlan.get("blossom-group-type").toString();
    blossomGroupItem->parent = parent;

    const JsonItem subTypeArray = growPlan.get("blossoms");
    if(subTypeArray.size() > 0)
    {
        for(uint32_t i = 0; i < subTypeArray.size(); i++)
        {
            JsonItem item = subTypeArray.get(i);

            BlossomItem* blossomItem = convertBlossom(item, parent);
            blossomItem->blossomPath = growPlan.get("b_path").toString();

            JsonItem array = growPlan.get("items-input");
            for(uint32_t i = 0; i < array.size(); i++)
            {
                const std::string key = array.get(i).get("key").toString();
                const std::string pairType = array.get(i).get("type").toString();
                JsonItem value = array.get(i).get("value");

                ValueItem itemValue;
                convertItemPart(value, itemValue, pairType);
                blossomItem->values.insert(key, itemValue);
            }

            blossomGroupItem->blossoms.push_back(blossomItem);
        }
    }
    else
    {
        BlossomItem* blossomItem =  new BlossomItem();
        blossomItem->blossomPath = growPlan.get("b_path").toString();
        blossomItem->blossomType = growPlan.get("blossom-group-type").toString();
        blossomItem->parent = parent;

        JsonItem array = growPlan.get("items-input");
        for(uint32_t i = 0; i < array.size(); i++)
        {
            const std::string key = array.get(i).get("key").toString();
            const std::string pairType = array.get(i).get("type").toString();
            JsonItem value = array.get(i).get("value");

            ValueItem itemValue;
            convertItemPart(value, itemValue, pairType);
            blossomItem->values.insert(key, itemValue);
        }

        blossomGroupItem->blossomGroupType = "special";
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
SakuraCompiler::convertBranch(JsonItem &growPlan,
                              SakuraItem* parent)
{
    BranchItem* branchItem = new BranchItem();
    branchItem->id = growPlan.get("id").toString();
    branchItem->parent = parent;

    JsonItem items = growPlan.get("items");

    if(growPlan.contains("items-input"))
    {
        JsonItem itemInput = growPlan.get("items-input");
        overrideItems(items, itemInput);
    }

    for(uint32_t i = 0; i < items.size(); i++)
    {
        const std::string key = items.get(i).get("key").toString();
        const std::string pairType = items.get(i).get("type").toString();
        JsonItem value = items.get(i).get("value");

        ValueItem itemValue;
        convertItemPart(value, itemValue, pairType);
        branchItem->values.insert(key, itemValue);
    }

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newMap.insert("b_path", growPlan.get("b_path"));
        branchItem->childs.push_back(convert(newMap, branchItem));
    }

    return branchItem;
}

/**
 * @brief SakuraCompiler::convertTree
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertTree(JsonItem &growPlan,
                            SakuraItem* parent)
{
    TreeItem* treeItem = new TreeItem();
    treeItem->id = growPlan.get("id").toString();
    treeItem->parent = parent;

    JsonItem items = growPlan.get("items");

    if(growPlan.contains("items-input"))
    {
        JsonItem itemInput = growPlan.get("items-input");
        overrideItems(items, itemInput);
    }

    for(uint32_t i = 0; i < items.size(); i++)
    {
        const std::string key = items.get(i).get("key").toString();
        const std::string pairType = items.get(i).get("type").toString();
        JsonItem value = items.get(i).get("value");

        ValueItem itemValue;
        convertItemPart(value, itemValue, pairType);
        treeItem->values.insert(key, itemValue);
    }

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        treeItem->childs.push_back(convert(newMap, treeItem));
    }

    return treeItem;
}

/**
 * @brief SakuraCompiler::convertForest
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertSeed(JsonItem &growPlan,
                            SakuraItem* parent)
{
    SeedItem* seedItem = new SeedItem();
    seedItem->name = growPlan.get("id").toString();
    seedItem->parent = parent;

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
SakuraCompiler::convertIf(JsonItem &growPlan,
                          SakuraItem* parent)
{
    IfBranching* newItem = new IfBranching();
    newItem->parent = parent;
    newItem->leftSide = growPlan.get("left").getItemContent()->copy()->toMap();
    newItem->rightSide = growPlan.get("right").getItemContent()->copy()->toMap();

    if(growPlan.get("if_type").getString() == "==") {
        newItem->ifType = IfBranching::EQUAL;
    }
    if(growPlan.get("if_type").getString() == "!=") {
        newItem->ifType = IfBranching::UNEQUAL;
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

    JsonItem if_parts = growPlan.get("if_parts");
    assert(if_parts.isValid());
    for(uint32_t i = 0; i < if_parts.size(); i++)
    {
        JsonItem newMap = if_parts.get(i);
        newMap.insert("b_path", growPlan.get("b_path"));
        newItem->ifChilds.push_back(convert(newMap, parent));
    }

    JsonItem else_parts = growPlan.get("else_parts");
    assert(else_parts.isValid());
    for(uint32_t i = 0; i < else_parts.size(); i++)
    {
        JsonItem newMap = else_parts.get(i);
        newMap.insert("b_path", growPlan.get("b_path"));
        newItem->elseChilds.push_back(convert(newMap, parent));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::convertForEach
 * @param growPlan
 * @param parent
 * @return
 */
SakuraItem*
SakuraCompiler::convertForEach(JsonItem &growPlan,
                               SakuraItem* parent)
{
    ForEachBranching* newItem = new ForEachBranching();
    newItem->parent = parent;
    newItem->tempVarName = growPlan.get("variable").getItemContent()->toString();

    JsonItem listItem = growPlan.get("list").getItemContent()->toMap();

    ValueItem itemValue;
    convertItemPart(listItem, itemValue, "assign");
    newItem->iterateArray.insert("array", itemValue);

    JsonItem content = growPlan.get("content");
    assert(content.isValid());
    for(uint32_t i = 0; i < content.size(); i++)
    {
        JsonItem newMap = content.get(i);
        newMap.insert("b_path", growPlan.get("b_path"));
        newItem->forChild.push_back(convert(newMap, parent));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::processSequeniellPart
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertSequeniellPart(JsonItem &growPlan,
                                      SakuraItem* parent)
{
    SequeniellBranching* newItem = new SequeniellBranching();
    newItem->parent = parent;

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newItem->childs.push_back(convert(newMap, parent));
    }

    return newItem;
}

/**
 * @brief SakuraCompiler::processParallelPart
 * @param growPlan
 * @return
 */
SakuraItem*
SakuraCompiler::convertParallelPart(JsonItem &growPlan,
                                    SakuraItem* parent)
{
    ParallelBranching* newItem = new ParallelBranching();
    newItem->parent = parent;

    JsonItem parts = growPlan.get("parts");
    assert(parts.isValid());

    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newItem->childs.push_back(convert(newMap, parent));
    }

    return newItem;
}

}
