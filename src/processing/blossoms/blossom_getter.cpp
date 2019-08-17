/**
 *  @file    blossom_getter.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "blossom_getter.h"
#include <processing/blossoms/blossom.h>

#include <processing/blossoms/install/apt/apt_absent_blossom.h>
#include <processing/blossoms/install/apt/apt_latest_blossom.h>
#include <processing/blossoms/install/apt/apt_present_blossom.h>
#include <processing/blossoms/install/apt/apt_update_blossom.h>
#include <processing/blossoms/install/apt/apt_upgrade_blossom.h>

namespace SakuraTree
{

/**
 * request a new blossom-object to process the requested task
 *
 * @return pointer to a new object or nullptr if type or subtype is unknown
 */
Blossom*
getBlossom(const std::string type,
           const std::string subType)
{
    if(type == "apt")
    {
        if(subType == "absent") {
            return new AptAbsentBlossom();
        }
        if(subType == "latest") {
            return new AptLatestBlossom();
        }
        if(subType == "present") {
            return new AptPresentBlossom();
        }
        if(subType == "update") {
            return new AptUdateBlossom();
        }
        if(subType == "upgrade") {
            return new AptUpgradeBlossom();
        }
    }

    return nullptr;
}

}
