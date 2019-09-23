/**
 *  @file    blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "blossom.h"
#include <sakura_root.h>
#include <items/item_methods.h>

namespace SakuraTree
{

Blossom::Blossom() {}

Blossom::~Blossom() {}

/**
 * @brief SakuraBlossom::growBlossom
 * @return
 */
void
Blossom::growBlossom(BlossomItem &blossomItem)
{
    std::vector<std::string> uninitItems = checkItems(&blossomItem.values);
    if(uninitItems.size() > 0)
    {
        std::string output = "The following items are not initialized: \n";
        for(uint32_t i = 0; i < uninitItems.size(); i++)
        {
            output += uninitItems.at(i) + "\n";
        }
        blossomItem.outputMessage = output;
        blossomItem.success = false;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"initTask"<<std::endl;
    }
    initTask(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_INIT_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"preCheck"<<std::endl;
    }
    preCheck(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_PRECHECK_STATE;
        return;
    }

    if(blossomItem.skip)
    {
        blossomItem.resultState = BlossomItem::SKIPPED_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"runTask"<<std::endl;
    }
    runTask(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_EXEC_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"postCheck"<<std::endl;
    }
    postCheck(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_POSTCHECK_STATE;

        return;
    }
    //-------------------------------
    if(DEBUG) {
        std::cout<<"closeTask"<<std::endl;
    }
    closeTask(blossomItem);
    if(blossomItem.success == false)
    {
        blossomItem.resultState = BlossomItem::ERROR_CLOSE_STATE;
        return;
    }

    //-------------------------------

    blossomItem.resultState = BlossomItem::CHANGED_STATE;
    return;
}

}
