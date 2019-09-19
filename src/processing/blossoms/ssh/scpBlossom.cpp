/**
 *  @file    scp_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "scp_blossom.h"

namespace SakuraTree
{

ScpBlossom::ScpBlossom()
{

}

/**
 * initTask
 */
void
ScpBlossom::initTask(BlossomData *blossomData)
{
    if(blossomData->items->contains("user") == false
            || blossomData->items->contains("address") == false)
    {
        blossomData->success = false;
        blossomData->outputMessage = "missing connection informations";
    }

    blossomData->success = true;
}

/**
 * preCheck
 */
void
ScpBlossom::preCheck(BlossomData *blossomData)
{
    //TODO: check per ssh if file already exist
    blossomData->success = true;
}

/**
 * runTask
 */
void
ScpBlossom::runTask(BlossomData *blossomData)
{
    std::string programm = "scp ";
    if(blossomData->items->contains("port")) {
        programm += " -P " + blossomData->items->getStringByKey("port");
    }
    if(blossomData->items->contains("ssh_key")) {
        programm += " -i " + blossomData->items->getStringByKey("ssh_key");
    }

    programm += blossomData->items->getStringByKey("user");
    programm += "@";
    programm += blossomData->items->getStringByKey("address");
    programm += ":";

    runSyncProcess(blossomData, programm);
}

/**
 * postCheck
 */
void
ScpBlossom::postCheck(BlossomData *blossomData)
{
    blossomData->success = true;
}

/**
 * closeTask
 */
void
ScpBlossom::closeTask(BlossomData *blossomData)
{
    blossomData->success = true;
}

}
