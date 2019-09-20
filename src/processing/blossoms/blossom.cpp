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
#include <items/sakura_items.h>
#include <items/item_methods.h>
#include <boost/algorithm/string/replace.hpp>

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

/**
 * @brief SakuraBlossom::runSyncProcess
 *
 * @return
 */
bool
Blossom::runSyncProcess(BlossomItem &blossomItem,
                        std::string command)
{
    std::vector<std::string> args;
    args.push_back("-c");
    boost::replace_all(command, "\"", "\\\"");
    args.push_back("\"" + command + "\"");

    return runSyncProcess(blossomItem, std::string("/bin/sh"), args);
}

/**
 * @brief Blossom::runSyncProcess
 *
 * @return
 */
bool
Blossom::runSyncProcess(BlossomItem &blossomItem,
                        const std::string &programm,
                        const std::vector<std::string> &args)
{
    std::string call = programm;
    for(uint32_t i = 0; i < args.size(); i++)
    {
        call += " " + args[i];
    }
    call.append(" 2>&1");

    if(DEBUG) {
        std::cout<<"call: "<<call<<std::endl;
    }

    std::string data = "";
    FILE* stream = nullptr;

    const uint32_t max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(call.c_str(), "r");

    if(stream)
    {
        while(!feof(stream))
        {
            if(fgets(buffer, max_buffer, stream) != nullptr) {
                data.append(buffer);
            }
        }
        blossomItem.execState = pclose(stream);
    }
    else
    {
        blossomItem.outputMessage = "can not execute programm: " + programm;
        blossomItem.success = false;

        return false;
    }

    std::cout<<"+++++++++++++++++++++++++++++++++++++++ exit status: "
             <<blossomItem.execState
             <<std::endl;

    if(data.size() < 10000){
        std::cout<<data<<std::endl;
    }

    blossomItem.outputMessage = data;
    if(blossomItem.execState == 0) {
        blossomItem.success = true;
    } else {
        blossomItem.success = false;
    }

    return true;
}

}
