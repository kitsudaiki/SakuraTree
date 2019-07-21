/**
 *  @file    blossomTypeHandler.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "blossomTypeHandler.h"
#include <processing/blossoms/blossom.h>

#include <processing/blossoms/install/apt/aptAbsentBlossom.h>
#include <processing/blossoms/install/apt/aptLatestBlossom.h>
#include <processing/blossoms/install/apt/aptPresentBlossom.h>
#include <processing/blossoms/install/apt/aptUpdateBlossom.h>
#include <processing/blossoms/install/apt/aptUpgradeBlossom.h>

using std::pair;
using std::string;
using std::map;

namespace SakuraSeed
{

BlossomTypeHandler::BlossomTypeHandler()
{

}

/**
 * @brief BlossomTypeHandler::initBlossoms
 */
void
BlossomTypeHandler::initBlossoms()
{
    map<string, Blossom*> aptBlossoms;

    aptBlossoms.insert(pair<string, Blossom*>("absent", new AptAbsentBlossom()));
    aptBlossoms.insert(pair<string, Blossom*>("latest", new AptLatestBlossom()));
    aptBlossoms.insert(pair<string, Blossom*>("present", new AptPresentBlossom()));
    aptBlossoms.insert(pair<string, Blossom*>("update", new AptUdateBlossom()));
    aptBlossoms.insert(pair<string, Blossom*>("upgrade", new AptUpgradeBlossom()));

    m_allBlossoms.insert(pair<string, map<string, Blossom*>>("apt", aptBlossoms));
}

/**
 * @brief BlossomTypeHandler::getBlossom
 * @param mainType
 * @param subType
 * @return
 */
Blossom*
BlossomTypeHandler::getBlossom(const string type,
                               const string subType)
{
    map<string, map<string, Blossom*>>::iterator itType;
    itType = m_allBlossoms.find(type);

    if(itType != m_allBlossoms.end())
    {
        map<string, Blossom*>::iterator itSubtype;
        itSubtype = itType->second.find(subType);

        if(itSubtype != itType->second.end()) {
            return itSubtype->second;
        }
    }

    return nullptr;
}

}
