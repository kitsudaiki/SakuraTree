/**
 *  @file    apt_methods.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_methods.h"

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/common_items/data_items.h>
#include <processing/process_methods.h>

namespace SakuraTree
{

/**
 * @brief AptBlossom::isInstalled
 * @param paket
 * @return
 */
bool
isInstalled(BlossomItem &blossomItem,
            const std::string &paket)
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
fillPackageNames(BlossomItem &blossomItem,
                 std::vector<std::string> &packageList)
{
    if(blossomItem.values.get("names")->isValue()) {
        packageList.push_back(blossomItem.values.getStringByKey("names"));
    }

    if(blossomItem.values.get("names")->isArray())
    {
        DataArray* tempItem = dynamic_cast<DataArray*>(blossomItem.values.get("names"));
        for(uint32_t i = 0; i < tempItem->size(); i++)
        {
            packageList.push_back(tempItem->get(i)->toString());
        }
    }
}

/**
 * @brief AptBlossom::createPackageList
 * @return
 */
std::string
createPackageList(const std::vector<std::string> &packageList)
{
    std::string result = "";
    for(uint32_t i = 0; i < packageList.size(); i++)
    {
        result += " " + packageList.at(i);
    }

    return result;
}

/**
 * @brief AptBlossom::getInstalledPackages
 * @param packageList
 * @return
 */
std::vector<std::string>
getInstalledPackages(BlossomItem &blossomItem,
                     const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomItem);

    for(uint32_t i = 0; i < packageList.size(); i++)
    {
        for(uint32_t j = 0; j < currentInstalled.size(); j++)
        {
            if(currentInstalled.at(j) == packageList.at(i))
            {
                result.push_back(packageList.at(i));
                continue;
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
getAbsendPackages(BlossomItem &blossomItem,
                  const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages(blossomItem);

    for(uint32_t i = 0; i < packageList.size(); i++)
    {
        bool found = false;
        for(uint32_t j = 0; j < currentInstalled.size(); j++)
        {
            if(currentInstalled.at(j) == packageList.at(i))
            {
                found = true;
                continue;
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
getInstalledPackages(BlossomItem &blossomItem)
{
    std::string command = "dpkg --list | grep ^ii  | awk ' {print \\$2} '";
    runSyncProcess(blossomItem, command);
    const std::string output(blossomItem.outputMessage);
    return Kitsunemimi::Common::splitStringByDelimiter(output, '\n');
}

}
