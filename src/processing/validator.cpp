/**
 * @file        validator.cpp
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

#include "validator.h"

#include <processing/blossoms/blossom_getter.h>
#include <processing/blossoms/blossom.h>
#include <sakura_root.h>

namespace SakuraTree
{

/**
 * @brief check if all values are set inside a blossom-item
 *
 * @param blossomItem blossom-item with the information
 *
 * @return true, if all necessary values are set, else false
 */
bool
checkBlossomItem(BlossomItem &blossomItem)
{
    bool result = false;

    Blossom* blossom = getBlossom(blossomItem.blossomGroupType, blossomItem.blossomType);
    if(blossom == nullptr)
    {
        SakuraRoot::m_root->createError(blossomItem, "converter", "unknow blossom-type");
        return false;
    }

    result = checkBlossomItem(blossomItem, blossom->m_requiredKeys);
    if(result) {
        result = checkOutput(blossomItem, blossom->m_hasOutput);
    }

    delete blossom;

    return result;
}

/**
 * @brief check, that only a output-variable is defined, when the blossom has output, which can
 *        be written into a variable
 *
 * @param blossomItem blossom-item with the information
 * @param hasOuput true, if blossom has output, which can be written into a variable
 *
 * @return true, if all necessary values are set, else false
 */
bool
checkOutput(BlossomItem &blossomItem,
            const bool hasOutput)
{
    std::map<std::string, ValueItem>::const_iterator it;
    for(it = blossomItem.values.m_valueMap.begin();
        it != blossomItem.values.m_valueMap.end();
        it++)
    {
        ValueItem tempItem = blossomItem.values.getValueItem(it->first);
        if(hasOutput == false
                && tempItem.type == ValueItem::OUTPUT_PAIR_TYPE)
        {
            // build error-output
            const std::string message = "variable \""
                                        + it->first
                                        + "\" is declared as output-variable, "
                                          "but the blossom has not"
                                          "output, which could be written into a variable.";
            SakuraRoot::m_root->createError(blossomItem,
                                            "converter",
                                            message);
            return false;
        }
    }

    return true;
}

/**
 * @brief check if all values of a blossom-item match with a list of required keys
 *
 * @param blossomItem blossom-item with the information
 * @param requiredKeys data-map with all required keys. The value behind each key is a
 *                     boolean data-value. If this value is false, the key is optional and don't
 *                     have to be in the values of the blossom-item
 *
 * @return true, if all necessary values are set, else false
 */
bool
checkBlossomItem(BlossomItem &blossomItem,
                 DataMap &requiredKeys)
{
    // if "*" is in the list of required key, there is more allowed as the list contains items
    // for example the print-blossom allows all key
    if(requiredKeys.contains("*") == false)
    {
        // check if all keys in the values of the blossom-item also exist in the required-key-list
        std::map<std::string, ValueItem>::const_iterator it;
        for(it = blossomItem.values.m_valueMap.begin();
            it != blossomItem.values.m_valueMap.end();
            it++)
        {
            ValueItem tempItem = blossomItem.values.getValueItem(it->first);
            if(tempItem.item == nullptr
                    && tempItem.type == ValueItem::INPUT_PAIR_TYPE)
            {
                // build error-output
                const std::string message = "variable \""
                                            + it->first
                                            + "\" is not in the list of allowed keys";
                SakuraRoot::m_root->createError(blossomItem,
                                                "converter",
                                                message);
                return false;
            }
        }
    }

    // check that all keys in the required keys are also in the values of the blossom-item
    std::map<std::string, DataItem*>::const_iterator it;
    for(it = requiredKeys.m_map.begin();
        it != requiredKeys.m_map.end();
        it++)
    {
        if(it->first == "*") {
            continue;
        }

        // if values of the blossom-item doesn't contain the key and the key is not optional,
        // then create an error-message
        if(blossomItem.values.contains(it->first) == false
                && it->second->toValue()->getBool() == true)
        {
            const std::string message = "variable \""
                                        + it->first
                                        + "\" is required, but is not set.";
            SakuraRoot::m_root->createError(blossomItem,
                                            "converter",
                                            message);
            return false;
        }
    }

    return true;
}


/**
 * @brief central method of the thread to check the current part of the execution-tree
 *
 * @param sakuraItem subtree, which should be checked
 *
 * @return true if successful, else false
 */
bool
checkSakuraItem(SakuraItem* sakuraItem,
                const std::string &filePath)
{
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SEQUENTIELL_ITEM)
    {
        SequentiellPart* sequential = dynamic_cast<SequentiellPart*>(sakuraItem);
        for(SakuraItem* item : sequential->childs)
        {
            if(checkSakuraItem(item, filePath) == false) {
                return false;
            }
        }
        return true;
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::TREE_ITEM)
    {
        TreeItem* treeItem = dynamic_cast<TreeItem*>(sakuraItem);
        const std::string completePath = treeItem->rootPath + "/" + treeItem->relativePath;
        return checkSakuraItem(treeItem->childs, completePath);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SUBTREE_ITEM)
    {
        return true;
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::BLOSSOM_ITEM)
    {
        BlossomItem* blossomItem = dynamic_cast<BlossomItem*>(sakuraItem);
        blossomItem->blossomPath = filePath;
        return checkBlossomItem(*blossomItem);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::BLOSSOM_GROUP_ITEM)
    {
        BlossomGroupItem* blossomGroupItem = dynamic_cast<BlossomGroupItem*>(sakuraItem);
        for(BlossomItem* blossomItem : blossomGroupItem->blossoms)
        {
            if(checkSakuraItem(blossomItem, filePath) == false) {
                return false;
            }
        }

        return true;
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::IF_ITEM)
    {
        IfBranching* ifBranching = dynamic_cast<IfBranching*>(sakuraItem);
        if(checkSakuraItem(ifBranching->ifContent, filePath) == false) {
            return false;
        }

        if(checkSakuraItem(ifBranching->elseContent, filePath) == false) {
            return false;
        }

        return true;
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::FOR_EACH_ITEM)
    {
        ForEachBranching* forEachBranching = dynamic_cast<ForEachBranching*>(sakuraItem);
        return checkSakuraItem(forEachBranching->content, filePath);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::FOR_ITEM)
    {
        ForBranching* forBranching = dynamic_cast<ForBranching*>(sakuraItem);
        return checkSakuraItem(forBranching->content, filePath);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::PARALLEL_ITEM)
    {
        ParallelPart* parallel = dynamic_cast<ParallelPart*>(sakuraItem);
        return checkSakuraItem(parallel->childs, filePath);
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SEED_ITEM)
    {
        return true;
    }
    //----------------------------------------------------------------------------------------------
    if(sakuraItem->getType() == SakuraItem::SEED_TRIGGER_ITEM)
    {
        return true;
    }
    //----------------------------------------------------------------------------------------------
    // TODO: error-message

    return false;
}

}
