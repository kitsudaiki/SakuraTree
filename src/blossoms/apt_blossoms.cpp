/**
 * @file        apt_blossoms.cpp
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

#include "apt_blossoms.h"

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiCommon/common_items/data_items.h>
#include <libKitsunemimiPersistence/logger/logger.h>

#include <sakura_root.h>

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

//==================================================================================================
// AptAbsentBlossom
//==================================================================================================
AptAbsentBlossom::AptAbsentBlossom()
    : Blossom()
{
    m_requiredKeys.insert("packages", new Kitsunemimi::DataValue(true));
}

/**
 * runTask
 */
bool
AptAbsentBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::vector<std::string> packageNames;

    DataArray* names = blossomItem.values.get("packages")->toArray();
    if(names != nullptr)
    {
        // list of packages
        for(uint32_t i = 0; i < names->size(); i++)
        {
            packageNames.push_back(names->get(i)->toString());
        }
    }
    else
    {
        // single package
        packageNames.push_back(blossomItem.values.get("packages")->toString());
    }

    // check if there are at least one package defined
    if(packageNames.size() == 0)
    {
        errorMessage = "no packages to defined";
        return false;
    }

    // check skip condition
    packageNames = getInstalledPackages(packageNames);
    if(packageNames.size() == 0) {
        return true;
    }

    // build command
    std::string command = "sudo apt-get remove -y ";
    for(const std::string& packageName : packageNames) {
        command += packageName + " ";
    }

    if(SakuraRoot::m_root->runCommand(command, errorMessage) == false) {
        return false;
    }

    // get list of still installed packages
    packageNames = getInstalledPackages(packageNames);

    // if there are still some packages left, create an error
    if(packageNames.size() > 0)
    {
        blossomItem.terminalOutput = "couldn't remove following packages: \n";
        for(const std::string& packageName : packageNames) {
            blossomItem.terminalOutput += "    " + packageName + "\n";
        }

        return false;
    }

    return true;
}

//==================================================================================================
// AptLatestBlossom
//==================================================================================================
AptLatestBlossom::AptLatestBlossom()
    : Blossom()
{
    m_requiredKeys.insert("packages", new Kitsunemimi::DataValue(true));
}

/**
 * @brief AptInstallBlossom::runTask
 */
bool
AptLatestBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::vector<std::string> packageNames;

    DataArray* names = blossomItem.values.get("packages")->toArray();
    if(names != nullptr)
    {
        // list of packages
        for(uint32_t i = 0; i < names->size(); i++) {
            packageNames.push_back(names->get(i)->toString());
        }
    }
    else
    {
        // single package
        packageNames.push_back(blossomItem.values.get("packages")->toString());
    }

    // check if there are at least one package defined
    if(packageNames.size() == 0)
    {
        errorMessage = "no packages to defined";
        return false;
    }

    // convert list into string
    std::string appendedList = "";
    for(const std::string& packageName : packageNames) {
        appendedList += packageName + " ";
    }

    // build command
    const std::string programm = "sudo apt-get install -y " + appendedList;
    if(SakuraRoot::m_root->runCommand(programm, errorMessage) == false) {
        return false;
    }

    // get list of not installed packages
    packageNames = getAbsendPackages(packageNames);

    // if there are still some packages missing, create an error
    if(packageNames.size() > 0)
    {
        blossomItem.terminalOutput = "couldn't install following packages: \n";
        for(const std::string& packageName : packageNames) {
            blossomItem.terminalOutput += packageName + "\n";
        }

        return false;
    }

    return true;
}

//==================================================================================================
// AptPresentBlossom
//==================================================================================================
AptPresentBlossom::AptPresentBlossom()
    : Blossom()
{
    m_requiredKeys.insert("packages", new Kitsunemimi::DataValue(true));
}

/**
 * @brief AptInstallBlossom::runTask
 */
bool
AptPresentBlossom::runTask(BlossomItem &blossomItem, std::string &errorMessage)
{
    std::vector<std::string> packageNames;

    DataArray* names = blossomItem.values.get("packages")->toArray();
    if(names != nullptr)
    {
        // list of packages
        for(uint32_t i = 0; i < names->size(); i++)
        {
            packageNames.push_back(names->get(i)->toString());
        }
    }
    else
    {
        // single package
        packageNames.push_back(blossomItem.values.get("packages")->toString());
    }

    // check if there are at least one package defined
    if(packageNames.size() == 0)
    {
        errorMessage = "no packages to defined";
        return false;
    }

    // check skip condition
    packageNames = getAbsendPackages(packageNames);
    if(packageNames.size() == 0) {
        return true;
    }

    // convert list into string
    std::string appendedList = "";
    for(const std::string& packageName : packageNames) {
        appendedList += packageName + " ";
    }

    // build command
    const std::string command = "sudo apt-get install -y " + appendedList;
    if(SakuraRoot::m_root->runCommand(command, errorMessage) == false) {
        return false;
    }

    // get list of not installed packages
    packageNames = getAbsendPackages(packageNames);

    // if there are still some packages missing, create an error
    if(packageNames.size() > 0)
    {
        blossomItem.terminalOutput = "couldn't install following packages: \n";
        for(const std::string& packageName : packageNames) {
            blossomItem.terminalOutput += "    " + packageName + "\n";
        }

        return false;
    }

    return true;
}

//==================================================================================================
// AptUdateBlossom
//==================================================================================================
AptUdateBlossom::AptUdateBlossom()
    : Blossom() {}

/**
 * runTask
 */
bool
AptUdateBlossom::runTask(BlossomItem &, std::string &errorMessage)
{
    const std::string command = "sudo apt-get update";
    return SakuraRoot::m_root->runCommand(command, errorMessage);
}

//==================================================================================================
// AptUpgradeBlossom
//==================================================================================================
AptUpgradeBlossom::AptUpgradeBlossom()
    : Blossom() {}


/**
 * runTask
 */
bool
AptUpgradeBlossom::runTask(BlossomItem &, std::string &errorMessage)
{
    const std::string command = "sudo apt-get -y upgrade";
    return SakuraRoot::m_root->runCommand(command, errorMessage);
}
