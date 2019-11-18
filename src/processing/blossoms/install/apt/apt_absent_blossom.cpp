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
#include <processing/blossoms/install/apt/apt_methods.h>

namespace SakuraTree
{

AptAbsentBlossom::AptAbsentBlossom() :
    Blossom() {}

/**
 * initTask
 */
void
AptAbsentBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues->contains("names") == false)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "no packages to defined";
        return;
    }

    fillPackageNames(blossomItem, m_packageNames);

    if(m_packageNames.size() == 0)
    {
        blossomItem.success = false;
        blossomItem.errorMessage = "no packages to defined";
        return;
    }

    blossomItem.success = true;
}

/**
 * preCheck
 */
void
AptAbsentBlossom::preCheck(BlossomItem &blossomItem)
{
    m_packageNames = getInstalledPackages(blossomItem, m_packageNames);
    if(m_packageNames.size() == 0) {
        blossomItem.skip = true;
    }

    blossomItem.success = true;
}

/**
 * runTask
 */
void
AptAbsentBlossom::runTask(BlossomItem &blossomItem)
{
    std::string appendedList = "";
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        appendedList += m_packageNames.at(i) + " ";
    }

    std::string programm = "sudo apt-get remove -y " + appendedList;
    runSyncProcess(blossomItem, programm);
}

/**
 * postCheck
 */
void
AptAbsentBlossom::postCheck(BlossomItem &blossomItem)
{
    m_packageNames = getInstalledPackages(blossomItem, m_packageNames);
    if(m_packageNames.size() > 0)
    {
        std::string output = "couldn't remove following packages: \n";
        for(uint32_t i = 0; i < m_packageNames.size(); i++)
        {
            output += m_packageNames.at(i) + "\n";
        }

        blossomItem.success = false;
        blossomItem.errorMessage = output;
        return;
    }

    blossomItem.success = true;
}

/**
 * closeTask
 */
void
AptAbsentBlossom::closeTask(BlossomItem &blossomItem)
{
    m_packageNames.clear();
    blossomItem.success = true;
}

}
