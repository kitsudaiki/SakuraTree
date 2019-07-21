/**
 *  @file    aptUpgradeBlosson.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "aptUpgradeBlossom.h"

namespace SakuraSeed
{

AptUpgradeBlossom::AptUpgradeBlossom() :
    AptBlossom() {}

/**
 * initTask
 */
void
AptUpgradeBlossom::initTask(BlossomData *blossomData)
{
    blossomData->success = true;
}

/**
 * preCheck
 */
void
AptUpgradeBlossom::preCheck(BlossomData *blossomData)
{
    blossomData->success = true;
}

/**
 * runTask
 */
void
AptUpgradeBlossom::runTask(BlossomData *blossomData)
{
    std::string programm = "sudo apt-get -y upgrade";
    runSyncProcess(blossomData, programm);
}

/**
 * postCheck
 */
void
AptUpgradeBlossom::postCheck(BlossomData *blossomData)
{
    blossomData->success = true;
}

/**
 * closeTask
 */
void
AptUpgradeBlossom::closeTask(BlossomData *blossomData)
{
    blossomData->success = true;
}

}
