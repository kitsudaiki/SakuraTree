/**
 *  @file    commonMethods.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "commonMethods.h"

#include <libKitsuneJinja2.h>
#include <sakuraRoot.h>
#include <processing/blossoms/blossom.h>

using Kitsune::Jinja2::KitsuneJinja2Converter;

namespace SakuraTree
{

/**
 * @brief convertString
 *
 * @return
 */
std::string
convertString(const std::string &templateString,
              JsonObject *content)
{
    if(content->isObject() == false) {
        return std::string("");
    }

    KitsuneJinja2Converter* converter = SakuraRoot::m_root->m_jinja2Converter;
    std::pair<std::string, bool> result = converter->convert(templateString, content);
    // TODO: handle false return value

    return result.first;
}

/**
 * replace the jinja2-converted values of the items-object with the stuff of the insertValues-object
 *
 * @return the original items-object, with all jinja2-content filled
 */
JsonObject*
fillItems(JsonObject* items,
          JsonObject* insertValues)
{
    const std::vector<std::string> keys = items->getKeys();

    for(uint32_t i = 0; i < keys.size(); i++)
    {
        JsonItem* obj = items->get(keys.at(i));
        if(obj->isValue())
        {
            const std::string tempItem = obj->toString();
            JsonValue* value = new JsonValue(convertString(tempItem, insertValues));
            items->insert(keys.at(i), value, true);
        }
    }

    return items;
}

/**
 * @brief overrideItems
 *
 * @return
 */
JsonObject*
overrideItems(JsonObject* original,
              JsonObject* override)
{
    const std::vector<std::string> keys = override->getKeys();
    for(uint32_t i = 0; i < keys.size(); i++)
    {
        original->insert(keys.at(i), override->get(keys.at(i))->copy(), true);
    }

    return original;
}

/**
 * @brief checkItems
 *
 * @return list of not initialized values
 */
std::vector<std::string>
checkItems(JsonObject* items)
{
    std::vector<std::string> result;

    const std::vector<std::string> keys = items->getKeys();

    for(uint32_t i = 0; i < keys.size(); i++)
    {
        if(items->get(keys.at(i))->toString() == "{{}}") {
            result.push_back(keys.at(i));
        }
    }

    return result;
}

/**
 * @brief printOutput
 * @param blossom
 */
void
printOutput(BlossomData* blossom)
{
    std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

    // print executeion-state
    switch(blossom->resultState)
    {
        case SKIPPED_STATE:
            std::cout<<"SKIPPED"<<std::endl;
            break;
        case CHANGED_STATE:
            std::cout<<"CHANGED"<<std::endl;
            break;
        case ERROR_INIT_STATE:
            std::cout<<"ERROR in init-state"<<std::endl;
            break;
        case ERROR_PRECHECK_STATE:
            std::cout<<"ERROR in pre-check-state"<<std::endl;
            break;
        case ERROR_EXEC_STATE:
            std::cout<<"ERROR in exec-state with error-code: "<<blossom->execState<<std::endl;
            break;
        case ERROR_POSTCHECK_STATE:
            std::cout<<"ERROR in post-check-state"<<std::endl;
            break;
        case ERROR_CLOSE_STATE:
            std::cout<<"ERROR in error-state"<<std::endl;
            break;
        default:
            break;
    }

    // print call-hirarchie
    for(uint32_t i = 0; i < blossom->nameHirarchie.size(); i++)
    {
        for(uint32_t j = 0; j < i; j++)
        {
            std::cout<<"   ";
        }
        std::cout<<blossom->nameHirarchie.at(i)<<std::endl;
    }

    // print process-output
    if(blossom->outputMessage.size() > 0
            && blossom->resultState >= 3)
    {
        std::cout<<std::endl;
        std::cout<<blossom->outputMessage<<std::endl;
    }

    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<std::endl;
}

}
