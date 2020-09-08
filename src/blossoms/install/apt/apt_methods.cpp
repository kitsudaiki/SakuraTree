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
#include <libKitsunemimiPersistence/logger/logger.h>

/**
 * @brief isInstalled check for a specific package, if this is installed
 *
 * @param package package-name to check
 *
 * @return true, if package is already installed, else false
 */
bool
isInstalled(const std::string &package)
{
    if(package.length() == 0) {
        return false;
    }

    const std::vector<std::string> currentInstalled = getInstalledPackages();
    for(const std::string& installedPackage : currentInstalled)
    {
        if(installedPackage == package) {
            return true;
        }
    }

    return false;
}

/**
 * @brief convert package-list into a whitespace separated string
 *
 * @param packageList package-list
 *
 * @return whitespace separated string
 */
const std::string
createPackageList(const std::vector<std::string> &packageList)
{
    std::string result = "";

    for(const std::string& package : packageList) {
        result += " " + package;
    }

    return result;
}

/**
 * @brief  check for a given list of package-names, which packages are already
 *         installed on the system
 *
 * @param packageList package-list for comparism
 *
 * @return list of packages of the given list, which are already installed
 */
const std::vector<std::string>
getInstalledPackages(const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages();

    for(const std::string& package : packageList)
    {
        for(const std::string& installedPackage : currentInstalled)
        {
            if(installedPackage == package)
            {
                result.push_back(package);
                break;
            }
        }
    }

    return result;
}

/**
 * @brief check for a given list of package-names, which packages are not installed on the system
 *
 * @param packageList package-list for comparism
 *
 * @return list of packages of the given list, which are not installed
 */
const std::vector<std::string>
getAbsendPackages(const std::vector<std::string> &packageList)
{
    std::vector<std::string> result;
    const std::vector<std::string> currentInstalled = getInstalledPackages();

    for(const std::string& package : packageList)
    {
        bool found = false;
        for(const std::string& installedPackage : currentInstalled)
        {
            if(installedPackage == package)
            {
                found = true;
                break;
            }
        }

        if(found == false) {
            result.push_back(package);
        }
    }

    return result;
}


/**
 * @brief get all with apt installed packages on the system
 *
 * @return list all all installed packages
 */
const std::vector<std::string>
getInstalledPackages()
{
    const std::string command = "dpkg --list | grep ^ii  | awk ' {print \\$2} '";
    LOG_DEBUG("run command: " + command);

    const ProcessResult processResult = runSyncProcess(command);
    // TODO: check for error
    std::vector<std::string> result;
    Kitsunemimi::splitStringByDelimiter(result, processResult.processOutput, '\n');

    return result;
}
