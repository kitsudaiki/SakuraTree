﻿/**
 *  @file    apt_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_blossom.h"
#include "apt_absent_blossom.h"
#include "apt_present_blossom.h"
#include "apt_update_blossom.h"
#include "apt_upgrade_blossom.h"
#include <items/item_methods.h>
#include <common_methods/string_methods.h>

#include <sakura_root.h>

namespace SakuraTree
{

AptBlossom::AptBlossom() :
    Blossom() {}

/**
 * @brief AptBlossom::isInstalled
 * @param paket
 * @return
 */
bool
AptBlossom::isInstalled(BlossomItem &blossomItem, const std::string &paket)
{
    if(paket.length() == 0) {
        return false;
    }

    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomItem);
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
AptBlossom::fillPackageNames(BlossomItem &blossomItem)
{
    if(blossomItem.values.get("names")->isValue()) {
        m_packageNames.push_back(blossomItem.values.getStringByKey("names"));
    }

    if(blossomItem.values.get("names")->isArray())
    {
        DataArray* tempItem = dynamic_cast<DataArray*>(blossomItem.values.get("names"));
        for(uint32_t i = 0; i < tempItem->size(); i++)
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
AptBlossom::getInstalledPackages(BlossomItem &blossomItem,
                                 const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomItem);

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
AptBlossom::getAbsendPackages(BlossomItem &blossomItem,
                              const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomItem);

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
AptBlossom::getInstalledPackages(BlossomItem &blossomItem)
{
    std::string command = "dpkg --list | grep ^ii  | awk ' {print $2} '";
    runSyncProcess(blossomItem, command);
    std::string output(blossomItem.outputMessage);

    remove_if(output.begin(), output.end(), isspace);

    return Kitsune::Common::splitStringByDelimiter(output, '\n');
}

}
