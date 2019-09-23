/**
 *  @file    apt_upgrade_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_upgrade_blossom.h"
#include <processing/blossoms/install/apt/apt_methods.h>
#include <processing/process_methods.h>

namespace SakuraTree
{

AptUpgradeBlossom::AptUpgradeBlossom() :
    Blossom() {}

/**
 * initTask
 */
void
AptUpgradeBlossom::initTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * preCheck
 */
void
AptUpgradeBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * runTask
 */
void
AptUpgradeBlossom::runTask(BlossomItem &blossomItem)
{
    std::string programm = "sudo apt-get -y upgrade";
    runSyncProcess(blossomItem, programm);
}

/**
 * postCheck
 */
void
AptUpgradeBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * closeTask
 */
void
AptUpgradeBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}
