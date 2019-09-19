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
ScpBlossom::initTask(BlossomItem *blossomItem)
{
    if(blossomItem->values.contains("user") == false
            || blossomItem->values.contains("address") == false)
    {
        blossomItem->success = false;
        blossomItem->outputMessage = "missing connection informations";
    }

    blossomItem->success = true;
}

/**
 * preCheck
 */
void
ScpBlossom::preCheck(BlossomItem *blossomItem)
{
    //TODO: check per ssh if file already exist
    blossomItem->success = true;
}

/**
 * runTask
 */
void
ScpBlossom::runTask(BlossomItem *blossomItem)
{
    std::string programm = "scp ";
    if(blossomItem->values.contains("port")) {
        programm += " -P " + blossomItem->values.getStringByKey("port");
    }
    if(blossomItem->values.contains("ssh_key")) {
        programm += " -i " + blossomItem->values.getStringByKey("ssh_key");
    }

    programm += blossomItem->values.getStringByKey("user");
    programm += "@";
    programm += blossomItem->values.getStringByKey("address");
    programm += ":";

    runSyncProcess(blossomItem, programm);
}

/**
 * postCheck
 */
void
ScpBlossom::postCheck(BlossomItem *blossomItem)
{
    blossomItem->success = true;
}

/**
 * closeTask
 */
void
ScpBlossom::closeTask(BlossomItem *blossomItem)
{
    blossomItem->success = true;
}

}
