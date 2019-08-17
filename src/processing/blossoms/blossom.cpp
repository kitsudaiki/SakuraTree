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
#include <common_methods.h>
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
Blossom::growBlossom(BlossomData *blossomData)
{
    std::vector<std::string> uninitItems = checkItems(blossomData->items);
    if(uninitItems.size() > 0)
    {
        std::string output = "The following items are not initialized: \n";
        for(uint32_t i = 0; i < uninitItems.size(); i++)
        {
            output += uninitItems.at(i) + "\n";
        }
        blossomData->outputMessage = output;
        blossomData->success = false;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"initTask"<<std::endl;
    }
    initTask(blossomData);
    if(blossomData->success == false)
    {
        blossomData->resultState = ERROR_INIT_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"preCheck"<<std::endl;
    }
    preCheck(blossomData);
    if(blossomData->success == false)
    {
        blossomData->resultState = ERROR_PRECHECK_STATE;
        return;
    }

    if(blossomData->skip)
    {
        blossomData->resultState = SKIPPED_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"runTask"<<std::endl;
    }
    runTask(blossomData);
    if(blossomData->success == false)
    {
        blossomData->resultState = ERROR_EXEC_STATE;
        return;
    }

    //-------------------------------
    if(DEBUG) {
        std::cout<<"postCheck"<<std::endl;
    }
    postCheck(blossomData);
    if(blossomData->success == false)
    {
        blossomData->resultState = ERROR_POSTCHECK_STATE;

        return;
    }
    //-------------------------------
    if(DEBUG) {
        std::cout<<"closeTask"<<std::endl;
    }
    closeTask(blossomData);
    if(blossomData->success == false)
    {
        blossomData->resultState = ERROR_CLOSE_STATE;
        return;
    }

    //-------------------------------

    blossomData->resultState = CHANGED_STATE;
    return;
}

/**
 * @brief SakuraBlossom::runSyncProcess
 *
 * @return
 */
bool
Blossom::runSyncProcess(BlossomData *blossomData,
                        std::string command)
{
    std::vector<std::string> args;
    args.push_back("-c");
    boost::replace_all(command, "\"", "\\\"");
    args.push_back("\"" + command + "\"");

    return runSyncProcess(blossomData, std::string("/bin/sh"), args);
}

/**
 * @brief Blossom::runSyncProcess
 *
 * @return
 */
bool
Blossom::runSyncProcess(BlossomData *blossomData,
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
        blossomData->execState = WEXITSTATUS(pclose(stream));
    }
    else
    {
        blossomData->outputMessage = "can not execute programm: " + programm;
        blossomData->success = false;

        return false;
    }

    std::cout<<"+++++++++++++++++++++++++++++++++++++++ exit status: "<<(int)blossomData->execState<<std::endl;
    if(data.size() < 10000){
        std::cout<<data<<std::endl;
    }
    blossomData->outputMessage = data;
    if(blossomData->execState == 0) {
        blossomData->success = true;
    } else {
        blossomData->success = false;
    }

    return true;
}

}
