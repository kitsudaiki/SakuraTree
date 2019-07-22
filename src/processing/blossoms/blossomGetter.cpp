/**
 *  @file    blossomGetter.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "blossomGetter.h"
#include <processing/blossoms/blossom.h>

#include <processing/blossoms/install/apt/aptAbsentBlossom.h>
#include <processing/blossoms/install/apt/aptLatestBlossom.h>
#include <processing/blossoms/install/apt/aptPresentBlossom.h>
#include <processing/blossoms/install/apt/aptUpdateBlossom.h>
#include <processing/blossoms/install/apt/aptUpgradeBlossom.h>

namespace SakuraSeed
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
