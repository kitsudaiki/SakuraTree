/**
 * @file        apt_methods.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "apt_methods.h"

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/common_items/data_items.h>
#include <processing/process_execution.h>

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
    if(blossomItem.inputValues.get("names")->isValue()) {
        packageList.push_back(blossomItem.inputValues.get("names")->getString());
    }

    if(blossomItem.inputValues.get("names")->isArray())
    {
        JsonItem tempItem = blossomItem.inputValues.get("names");
        for(uint32_t i = 0; i < tempItem.size(); i++)
        {
            packageList.push_back(tempItem.get(i).toString());
        }
    }
}

/**
 * @brief AptBlossom::createPackageList
 * @return
 */
const std::string
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
const std::vector<std::string>
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
const std::vector<std::string>
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
const std::vector<std::string>
getInstalledPackages(BlossomItem &blossomItem)
{
    std::string command = "dpkg --list | grep ^ii  | awk ' {print \\$2} '";
    BlossomItem tempItem;
    runSyncProcess(tempItem, command);
    const std::string output(tempItem.processOutput);
    return Kitsunemimi::Common::splitStringByDelimiter(output, '\n');
}

}
