/**
 * @file        converter.cpp
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

#include "converter.h"

#include <libKitsunemimiSakuraParser/sakura_parsing.h>
#include <libKitsunemimiSakuraParser/sakura_parsing.h>

#include <libKitsunemimiJson/json_item.h>

#include <sakura_root.h>
#include <items/sakura_items.h>
#include <processing/common/item_methods.h>

#include <branch_builder/provision_branch_builder.h>

using Kitsunemimi::Json::JsonItem;

namespace SakuraTree
{

/**
 * @brief constructor
 */
Converter::Converter() {}

/**
 * @brief destructor
 */
Converter::~Converter() {}

/**
 * @brief SakuraCompiler::compile
 * @param tree
 * @return
 */
SakuraItem*
Converter::convert(const JsonItem &tree)
{
    // debug-output
    if(DEBUG)
    {
        std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        const std::string output = tree.toString(true);
        std::cout<<output<<std::endl;
        std::cout<<"-----------------------------------------------------"<<std::endl;
    }

    SakuraItem* result = convertPart(tree);

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
Converter::convertItemPart(ValueItem &resultingPart,
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
    if(itemInput.get("b_type").toString() == "identifier") {
        resultingPart.isIdentifier = true;
    }

    // get function
    JsonItem functions = itemInput.get("functions");
    for(uint32_t f = 0; f < functions.size(); f++)
    {
        FunctionItem functionItem;

        // get function-type
        if(functions.get(f).get("b_type").toString() == "get") {
            functionItem.type = FunctionItem::GET_FUNCTION;
        }
        if(functions.get(f).get("b_type").toString() == "split") {
            functionItem.type = FunctionItem::SPLIT_FUNCTION;
        }
        if(functions.get(f).get("b_type").toString() == "contains") {
            functionItem.type = FunctionItem::CONTAINS_FUNCTION;
        }
        if(functions.get(f).get("b_type").toString() == "size") {
            functionItem.type = FunctionItem::SIZE_FUNCTION;
        }
        if(functions.get(f).get("b_type").toString() == "insert") {
            functionItem.type = FunctionItem::INSERT_FUNCTION;
        }
        if(functions.get(f).get("b_type").toString() == "append") {
            functionItem.type = FunctionItem::APPEND_FUNCTION;
        }

        // get argument-list
        JsonItem arguments = functions.get(f).get("args");
        for(uint32_t a = 0; a < arguments.size(); a++)
        {
            ValueItem newItem;
            convertItemPart(newItem, arguments.get(a), "assign");
            functionItem.arguments.push_back(newItem);
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
Converter::overrideItems(JsonItem &original,
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
Converter::convertValues(SakuraItem* obj,
                         const JsonItem &values)
{
    for(uint32_t i = 0; i < values.size(); i++)
    {
        const std::string key = values.get(i).get("key").toString();
        const std::string pairType = values.get(i).get("b_type").toString();
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
Converter::convertPart(const JsonItem &subtree)
{
    const std::string typeName = subtree.get("b_type").toString();

    if(typeName == "blossom_group") {
        return convertBlossomGroup(subtree);
    }

    if(typeName == "subtree") {
        return convertSubtree(subtree);
    }

    if(typeName == "seed") {
        return convertSeed(subtree);
    }

    if(typeName == "if") {
        return convertIf(subtree);
    }

    if(typeName == "for_each") {
        return convertForEach(subtree, false);
    }

    if(typeName == "for") {
        return convertFor(subtree, false);
    }

    if(typeName == "parallel") {
        return convertParallel(subtree);
    }

    if(typeName == "parallel_for") {
        return convertFor(subtree, true);
    }

    if(typeName == "parallel_for_each") {
        return convertForEach(subtree, true);
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
Converter::convertBlossomGroup(const JsonItem &subtree)
{
    // init new blossom-group-item
    BlossomGroupItem* blossomGroupItem =  new BlossomGroupItem();
    blossomGroupItem->id = subtree.get("name").toString();
    blossomGroupItem->blossomGroupType = subtree.get("blossom-group-type").toString();

    // convert all blossoms of the group
    const JsonItem subTypeArray = subtree.get("blossoms");
    if(subTypeArray.size() > 0)
    {
        for(uint32_t i = 0; i < subTypeArray.size(); i++)
        {
            const JsonItem item = subTypeArray.get(i);

            // init new blossom-item
            BlossomItem* blossomItem = new BlossomItem();
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
Converter::convertSubtree(const JsonItem &subtree)
{
    // init new branch-item
    SubtreeItem* branchItem = new SubtreeItem();
    branchItem->id = subtree.get("b_id").toString();

    // fill values with the input of the upper level and convert the result
    JsonItem items = subtree.get("items");
    if(subtree.contains("items-input")) {
        overrideItems(items, subtree.get("items-input"));
    }
    convertValues(branchItem, items);

    // convert parts of the branch
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        branchItem->childs.push_back(convertPart(newMap));
    }

    return branchItem;
}

/**
 * @brief convert seed-object
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
Converter::convertSeed(const JsonItem &subtree)
{
    // init new seed-item
    SeedItem* seedItem = new SeedItem();
    seedItem->name = subtree.get("b_id").toString();

    // generate new branch-item based on the information
    const JsonItem connectionInfos = subtree.get("connection");
    SubtreeItem* provisioningBranch = createProvisionBranch
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
Converter::convertIf(const JsonItem &subtree)
{
    // init new if-item
    IfBranching* newItem = new IfBranching();

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
    Sequentiell* sequentiellContentIf = new Sequentiell();
    for(uint32_t i = 0; i < if_parts.size(); i++)
    {
        JsonItem newMap = if_parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        sequentiellContentIf->childs.push_back(convertPart(newMap));
    }
    newItem->ifContent = sequentiellContentIf;

    // convert else-part
    JsonItem else_parts = subtree.get("else_parts");
    assert(else_parts.isValid());
    Sequentiell* sequentiellContentElse = new Sequentiell();
    for(uint32_t i = 0; i < else_parts.size(); i++)
    {
        JsonItem newMap = else_parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        sequentiellContentElse->childs.push_back(convertPart(newMap));
    }
    newItem->elseContent = sequentiellContentElse;

    return newItem;
}

/**
 * @brief convert for-each-loop
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
Converter::convertForEach(const JsonItem &subtree,
                          bool parallel)
{
    // init new for-each-item
    ForEachBranching* newItem = new ForEachBranching(parallel);
    newItem->tempVarName = subtree.get("variable").getItemContent()->toString();

    // convert the item, over which should be iterated
    ValueItem itemValue;
    convertItemPart(itemValue, subtree.get("list"), "assign");
    newItem->iterateArray.insert("array", itemValue);
    convertValues(newItem, subtree.get("items"));

    // convert parts of the for-loop
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());

    Sequentiell* sequentiellContent = new Sequentiell();
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        sequentiellContent->childs.push_back(convertPart(newMap));
    }
    newItem->content = sequentiellContent;

    return newItem;
}

/**
 * @brief convert for-loop
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
Converter::convertFor(const JsonItem &subtree,
                           bool parallel)
{
    // init new for-item
    ForBranching* newItem = new ForBranching(parallel);
    newItem->tempVarName = subtree.get("variable1").getItemContent()->toString();

    // convert start- and end-point of the iterations
    convertItemPart(newItem->start, subtree.get("start"), "assign");
    convertItemPart(newItem->end, subtree.get("end"), "assign");
    convertValues(newItem, subtree.get("items"));


    // convert parts of the for-loop
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());

    Sequentiell* sequentiellContent = new Sequentiell();
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        JsonItem newMap = parts.get(i);
        newMap.insert("b_path", subtree.get("b_path"));
        sequentiellContent->childs.push_back(convertPart(newMap));
    }
    newItem->content = sequentiellContent;

    return newItem;
}

/**
 * @brief convert parallel part of branch
 * @param subtree current suttree for converting
 * @param parent pointer ot the parent-branch or -tree
 * @return pointer of the current converted part
 */
SakuraItem*
Converter::convertParallel(const JsonItem &subtree)
{
    // init new parallel-branching-item
    Parallel* newItem = new Parallel();

    // convert parts of the tree
    const JsonItem parts = subtree.get("parts");
    assert(parts.isValid());
    for(uint32_t i = 0; i < parts.size(); i++)
    {
        newItem->childs.push_back(convertPart(parts.get(i)));
    }

    return newItem;
}

}
