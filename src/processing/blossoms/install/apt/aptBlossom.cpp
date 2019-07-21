/**
 *  @file    aptBlossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "aptBlossom.h"
#include "aptAbsentBlossom.h"
#include "aptPresentBlossom.h"
#include "aptUpdateBlossom.h"
#include "aptUpgradeBlossom.h"
#include <commonMethods.h>
#include <commonMethods/stringMethods.h>

#include <sakuraRoot.h>

namespace SakuraSeed
{

AptBlossom::AptBlossom() :
    Blossom() {}

/**
 * @brief AptBlossom::isInstalled
 * @param paket
 * @return
 */
bool
AptBlossom::isInstalled(BlossomData* blossomData, const std::string &paket)
{
    if(paket.length() == 0) {
        return false;
    }

    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomData);
    for(uint32_t i = 0; i < currentInstalled.size(); i++)
    {
        if(currentInstalled.at(i) == paket) {
            return true;
        }
    }

    return false;
}

/**
 * @brief AptBlossom::fillPackageNames
 */
void
AptBlossom::fillPackageNames(BlossomData* blossomData)
{
    if(blossomData->items->get("names")->isValue()) {
        m_packageNames.push_back(blossomData->items->getString("names"));
    }

    if(blossomData->items->get("names")->isArray())
    {
        JsonArray* tempItem = dynamic_cast<JsonArray*>(blossomData->items->get("names"));
        for(uint32_t i = 0; i < tempItem->getSize(); i++)
        {
            m_packageNames.push_back(tempItem->get(i)->toString());
        }
    }
}

/**
 * @brief AptBlossom::createPackageList
 * @return
 */
std::string
AptBlossom::createPackageList()
{
    std::string result = "";
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        result += " " + m_packageNames.at(i);
    }

    return result;
}

/**
 * @brief AptBlossom::getInstalledPackages
 * @param packageList
 * @return
 */
std::vector<std::string>
AptBlossom::getInstalledPackages(BlossomData* blossomData,
                                 const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomData);

    for(uint32_t i = 0; i < packageList.size(); i++)
    {
        for(uint32_t j = 0; j < currentInstalled.size(); j++)
        {
            if(currentInstalled.at(j) == packageList.at(i)) {
                result.push_back(packageList.at(j));
            }
        }
    }

    return result;
}

/**
 * @brief AptBlossom::getAbsendPackages
 * @param packageList
 * @return
 */
std::vector<std::string>
AptBlossom::getAbsendPackages(BlossomData* blossomData,
                              const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomData);

    for(uint32_t i = 0; i < packageList.size(); i++)
    {
        bool found = false;
        for(uint32_t j = 0; j < currentInstalled.size(); j++)
        {
            if(currentInstalled.at(j) == packageList.at(i)) {
                found = true;
            }
        }

        if(found == false) {
            result.push_back(packageList.at(i));
        }
    }

    return result;
}


/**
 * @brief AptBlossom::getInstalledPackages
 * @return
 */
std::vector<std::string>
AptBlossom::getInstalledPackages(BlossomData* blossomData)
{
    std::string command = "dpkg --list | grep ^ii  | awk ' {print $2} '";
    runSyncProcess(blossomData, command);
    std::string output(blossomData->outputMessage);

    remove_if(output.begin(), output.end(), isspace);

    return Kitsune::splitString(output, '\n');
}

}
