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

AptAbsentBlossom::AptAbsentBlossom()
    : Blossom()
{
    m_requiredKeys.insert("packages", new Kitsunemimi::DataValue(true));
}

Kitsunemimi::Sakura::Blossom*
AptAbsentBlossom::createNewInstance()
{
    return new AptAbsentBlossom();
}

/**
 * initBlossom
 */
void
AptAbsentBlossom::initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    DataArray* names = blossomItem.values.get("packages")->toArray();
    if(names != nullptr)
    {
        // list of packages
        for(uint32_t i = 0; i < names->size(); i++)
        {
            m_packageNames.push_back(names->get(i)->toString());
        }
    }
    else
    {
        // single package
        m_packageNames.push_back(blossomItem.values.get("packages")->toString());
    }

    // check if there are at least one package defined
    if(m_packageNames.size() == 0)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "no packages to defined";
        return;
    }

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
AptAbsentBlossom::preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    m_packageNames = getInstalledPackages(m_packageNames);

    if(m_packageNames.size() == 0) {
        blossomItem.skip = true;
    }

    blossomItem.success = true;
}

/**
 * runTask
 */
void
AptAbsentBlossom::runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    // convert list into string
    std::string appendedList = "";
    for(const std::string& packageName : m_packageNames)
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
}

/**
 * postCheck
 */
void
AptAbsentBlossom::postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    // get list of still installed packages
    m_packageNames = getInstalledPackages(m_packageNames);

    // if there are still some packages left, create an error
    if(m_packageNames.size() > 0)
    {
        std::string output = "couldn't remove following packages: \n";
        for(const std::string& packageName : m_packageNames)
        {
            output += "    " + packageName + "\n";
        }

        blossomItem.success = false;
        blossomItem.outputMessage = output;

        return;
    }

    blossomItem.success = true;
}

/**
 * closeBlossom
 */
void
AptAbsentBlossom::closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem)
{
    m_packageNames.clear();
    blossomItem.success = true;
}
