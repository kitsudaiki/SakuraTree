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
 * @param tree
 * @return
 */
SakuraItem*
SakuraCompiler::compile(const JsonItem &tree)
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
 * @param resultingPart
 * @param itemInput
 * @param pairType
 * @return
 */
bool
SakuraCompiler::convertItemPart(ValueItem &resultingPart,
                                JsonItem itemInput,
                                const std::string pairType)
{
    if(itemInput.isMap())
    {
        resultingPart.item = itemInput.get("item").getItemContent()->copy()->toValue();
    }
    else if (itemInput.isArray())
    {
        DataArray* tempArray = new DataArray();
        for(uint32_t i = 0; i < itemInput.size(); i++)
        {
            tempArray->append(itemInput.get(i).getItemContent()->copy());
        }
        resultingPart.item = tempArray;
    }

    // set type
    if(pairType == "assign") {
        resultingPart.type = ValueItem::INPUT_PAIR_TYPE;
    }
    if(pairType == "compare") {
        resultingPart.type = ValueItem::COMPARE_EQUAL_PAIR_TYPE;
    }
    if(pairType == "output") {
        resultingPart.type = ValueItem::OUTPUT_PAIR_TYPE;
    }

    // check if current value is value or identifier
    if(itemInput.get("type").toString() == "identifier") {
        resultingPart.isIdentifier = true;
    }

    // get function
    JsonItem functions = itemInput.get("functions");
    for(uint32_t f = 0; f < functions.size(); f++)
    {
        FunctionItem functionItem;

        // get function-type
        if(functions.get(f).get("m_type").toString() == "get") {
            functionItem.type = FunctionItem::GET_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "split") {
            functionItem.type = FunctionItem::SPLIT_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "contains") {
            functionItem.type = FunctionItem::CONTAINS_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "size") {
            functionItem.type = FunctionItem::SIZE_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "insert") {
            functionItem.type = FunctionItem::INSERT_FUNCTION;
        }
        if(functions.get(f).get("m_type").toString() == "append") {
            functionItem.type = FunctionItem::APPEND_FUNCTION;
        }

        // get argument-list
        JsonItem arguments = functions.get(f).get("args");
        for(uint32_t a = 0; a < arguments.size(); a++)
        {
            DataValue* arg = arguments.get(a).getItemContent()->toValue();
            functionItem.arguments.push_back(*arg);
        }

        resultingPart.functions.push_back(functionItem);
    }

    return true;
}

/**
 * @brief merge two item-maps
 * @param original original item-map
 * @param override additional item-map for merging into the original one
 */
void
SakuraCompiler::overrideItems(JsonItem &original,
                              const JsonItem &override)
{
    DataMap* overrideMap = override.getItemContent()->toMap();
    std::map<std::string, DataItem*>::const_iterator it;
    for(it = overrideMap->m_map.begin();
        it != overrideMap->m_map.end();
        it++)
    {
        original.insert(it->first,
                        it->second->copy(),
                        true);
    }
}

/**
 * @brief SakuraCompiler::convertValues
 * @param obj
 * @param subtree
 */
void
SakuraCompiler::convertValues(SakuraItem* obj,
                              const JsonItem &values)
{
    for(uint32_t i = 0; i < values.size(); i++)
    {
        const std::string key = values.get(i).get("key").toString();
        const std::string pairType = values.get(i).get("type").toString();
        const JsonItem value = values.get(i).get("value");

        ValueItem itemValue;
        convertItemPart(itemValue, value, pairType);
        obj->values.insert(key, itemValue);
    }
}

/**
 * @brief convert subtree
 * @param subtree current subtree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convert(const JsonItem &subtree,
                        SakuraItem* parent)
{
    const std::string typeName = subtree.get("b_type").toString();

    if(typeName == "blossom_group") {
        return convertBlossomGroup(subtree, parent);
    }

    if(typeName == "branch") {
        return convertBranch(subtree, parent);
    }

    if(typeName == "tree") {
        return convertTree(subtree, parent);
    }

    if(typeName == "sequentiell") {
        return convertSequeniellPart(subtree, parent);
    }

    if(typeName == "parallel") {
        return convertParallelPart(subtree, parent);
    }

    if(typeName == "seed") {
        return convertSeed(subtree, parent);
    }

    if(typeName == "if") {
        return convertIf(subtree, parent);
    }

    if(typeName == "for_each") {
        return convertForEach(subtree, parent);
    }

    if(typeName == "for") {
        return convertFor(subtree, parent);
    }

    // it must everytime match one of the names
    assert(false);

    return nullptr;
}

/**
 * @brief convert blossom-group
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertBlossomGroup(const JsonItem &subtree,
                                    SakuraItem* parent)
{
    // init new blossom-group-item
    BlossomGroupItem* blossomGroupItem =  new BlossomGroupItem();
    blossomGroupItem->id = subtree.get("name").toString();
    blossomGroupItem->blossomGroupType = subtree.get("blossom-group-type").toString();
    blossomGroupItem->parent = parent;

    // convert all blossoms of the group
    const JsonItem subTypeArray = subtree.get("blossoms");
    if(subTypeArray.size() > 0)
    {
        for(uint32_t i = 0; i < subTypeArray.size(); i++)
        {
            const JsonItem item = subTypeArray.get(i);

            // init new blossom-item
            BlossomItem* blossomItem = new BlossomItem();
            blossomItem->parent = parent;
            blossomItem->blossomType = item.get("blossom-type").toString();
            blossomItem->blossomPath = subtree.get("b_path").toString();

            convertValues(blossomItem, subtree.get("items-input"));
            convertValues(blossomItem, item.get("items-input"));

            blossomGroupItem->blossoms.push_back(blossomItem);
        }
    }
    else
    {
        // init new blossom-item
        BlossomItem* blossomItem =  new BlossomItem();
        blossomItem->blossomPath = subtree.get("b_path").toString();
        blossomItem->blossomType = subtree.get("blossom-group-type").toString();
        blossomItem->parent = parent;

        convertValues(blossomItem, subtree.get("items-input"));

        blossomGroupItem->blossomGroupType = "special";
        blossomGroupItem->blossoms.push_back(blossomItem);
    }

    return blossomGroupItem;
}

/**
 * @brief convert branch
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertBranch(const JsonItem &subtree,
                              SakuraItem* parent)
{
    // init new branch-item
    BranchItem* branchItem = new BranchItem();
    branchItem->id = subtree.get("id").toString();
    branchItem->parent = parent;

    // fill values with the input of the upper level and convert the result
    JsonItem items = subtree.get("items");
    if(subtree.contains("items-input")) {
        overrideItems(items, subtree.get("items-input"));
    }
    convertValues(branchItem,items);

    // convert parts of the branch
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        branchItem->childs.push_back(convert(newMap, branchItem));
    }

    return branchItem;
}

/**
 * @brief convert tree
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertTree(const JsonItem &subtree,
                            SakuraItem* parent)
{
    // init new tree-item
    TreeItem* treeItem = new TreeItem();
    treeItem->id = subtree.get("id").toString();
    treeItem->parent = parent;

    // fill values with the input of the upper level and convert the result
    JsonItem items = subtree.get("items");
    if(subtree.contains("items-input")) {
        overrideItems(items, subtree.get("items-input"));
    }
    convertValues(treeItem,items);

    // convert parts of the tree
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        treeItem->childs.push_back(convert(newMap, treeItem));
    }

    return treeItem;
}

/**
 * @brief convert seed-object
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertSeed(const JsonItem &subtree,
                            SakuraItem* parent)
{
    // init new seed-item
    SeedItem* seedItem = new SeedItem();
    seedItem->name = subtree.get("id").toString();
    seedItem->parent = parent;

    // generate new branch-item based on the information
    const JsonItem connectionInfos = subtree.get("connection");
    BranchItem* provisioningBranch = createProvisionBranch
            (
                connectionInfos.get("address").getString(),
                connectionInfos.get("ssh_port").getInt(),
                connectionInfos.get("ssh_user").getString(),
                connectionInfos.get("ssh_key").getString(),
                SakuraRoot::m_executablePath,
                "",
                subtree.get("subtree").getString()
            );

    seedItem->child = provisioningBranch;

    return seedItem;
}

/**
 * @brief convert if-condition
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertIf(const JsonItem &subtree,
                          SakuraItem* parent)
{
    // init new if-item
    IfBranching* newItem = new IfBranching();
    newItem->parent = parent;

    // convert the both sides of the if-condition
    convertItemPart(newItem->leftSide, subtree.get("left"), "assign");
    convertItemPart(newItem->rightSide, subtree.get("right"), "assign");

    // convert compare-tpye
    if(subtree.get("if_type").getString() == "==") {
        newItem->ifType = IfBranching::EQUAL;
    }
    if(subtree.get("if_type").getString() == "!=") {
        newItem->ifType = IfBranching::UNEQUAL;
    }
    if(subtree.get("if_type").getString() == ">=") {
        newItem->ifType = IfBranching::GREATER_EQUAL;
    }
    if(subtree.get("if_type").getString() == ">") {
        newItem->ifType = IfBranching::GREATER;
    }
    if(subtree.get("if_type").getString() == "<=") {
        newItem->ifType = IfBranching::SMALLER_EQUAL;
    }
    if(subtree.get("if_type").getString() == "<") {
        newItem->ifType = IfBranching::SMALLER;
    }

    // convert if-part
    JsonItem if_parts = subtree.get("if_parts");
    assert(if_parts.isValid());
    for(uint32_t i = 0; i < if_parts.size(); i++)
    {
        JsonItem newMap = if_parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        newItem->ifChilds.push_back(convert(newMap, parent));
    }

    // convert else-part
    JsonItem else_parts = subtree.get("else_parts");
    assert(else_parts.isValid());
    for(uint32_t i = 0; i < else_parts.size(); i++)
    {
        JsonItem newMap = else_parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        newItem->elseChilds.push_back(convert(newMap, parent));
    }

    return newItem;
}

/**
 * @brief convert for-each-loop
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertForEach(const JsonItem &subtree,
                               SakuraItem* parent)
{
    // init new for-each-item
    ForEachBranching* newItem = new ForEachBranching();
    newItem->parent = parent;
    newItem->tempVarName = subtree.get("variable").getItemContent()->toString();

    // convert the item, over which should be iterated
    ValueItem itemValue;
    convertItemPart(itemValue, subtree.get("list"), "assign");
    newItem->iterateArray.insert("array", itemValue);

    // convert content of the for-loop
    const JsonItem content = subtree.get("content");
    assert(content.isValid());
    for(uint32_t i = 0; i < content.size(); i++)
    {
        JsonItem newMap = content.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        newItem->forChild.push_back(convert(newMap, parent));
    }

    return newItem;
}

/**
 * @brief convert for-loop
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertFor(const JsonItem &subtree,
                           SakuraItem* parent)
{
    // init new for-item
    ForBranching* newItem = new ForBranching();
    newItem->parent = parent;
    newItem->tempVarName = subtree.get("variable1").getItemContent()->toString();

    // convert start- and end-point of the iterations
    convertItemPart(newItem->start, subtree.get("start"), "assign");
    convertItemPart(newItem->end, subtree.get("end"), "assign");

    // convert content of the for-loop
    const JsonItem content = subtree.get("content");
    assert(content.isValid());
    for(uint32_t i = 0; i < content.size(); i++)
    {
        JsonItem newMap = content.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        newItem->forChild.push_back(convert(newMap, parent));
    }

    return newItem;
}

/**
 * @brief convert sequentiell part of tree
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertSequeniellPart(const JsonItem &subtree,
                                      SakuraItem* parent)
{
    // init new sequentiell-branching-item
    SequentiellBranching* newItem = new SequentiellBranching();
    newItem->parent = parent;

    // convert parts of the tree
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        newItem->childs.push_back(convert(parts.get(i), parent));
    }

    return newItem;
}

/**
 * @brief convert parallel part of tree
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
SakuraCompiler::convertParallelPart(const JsonItem &subtree,
                                    SakuraItem* parent)
{
    // init new parallel-branching-item
    ParallelBranching* newItem = new ParallelBranching();
    newItem->parent = parent;

    // convert parts of the tree
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        newItem->childs.push_back(convert(parts.get(i), parent));
    }

    return newItem;
}

}
