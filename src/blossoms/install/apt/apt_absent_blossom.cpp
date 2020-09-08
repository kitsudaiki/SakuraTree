/**
 * @file        apt_absent_blossom.cpp
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

#include "apt_absent_blossom.h"
#include <blossoms/install/apt/apt_methods.h>

/**
 * @brief constructor
 */
AptAbsentBlossom::AptAbsentBlossom()
    : Blossom()
{
    m_requiredKeys.insert("packages", new Kitsunemimi::DataValue(true));
}

/**
 * runTask
 */
void
AptAbsentBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
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
        blossomItem.success = false;
        blossomItem.outputMessage = "no packages to defined";
        return;
    }


    packageNames = getInstalledPackages(packageNames);

    if(packageNames.size() == 0) {
        blossomItem.skip = true;
    }

    // convert list into string
    std::string appendedList = "";
    for(const std::string& packageName : packageNames)
    {
        appendedList += packageName + " ";
    }

    // build command
    const std::string programm = "sudo apt-get remove -y " + appendedList;
    LOG_DEBUG("run command: " + programm);

    // run command
    ProcessResult processResult = runSyncProcess(programm);
    blossomItem.success = processResult.success;
    blossomItem.outputMessage = processResult.processOutput;

    // get list of still installed packages
    packageNames = getInstalledPackages(packageNames);

    // if there are still some packages left, create an error
    if(packageNames.size() > 0)
    {
        std::string output = "couldn't remove following packages: \n";
        for(const std::string& packageName : packageNames)
        {
            output += "    " + packageName + "\n";
        }

        blossomItem.success = false;
        blossomItem.outputMessage = output;

        return;
    }

    blossomItem.success = true;
}
