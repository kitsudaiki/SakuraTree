/**
 * @file        apt_latest_blossom.cpp
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

#include "apt_latest_blossom.h"
#include <processing/blossoms/install/apt/apt_methods.h>

namespace SakuraTree
{

AptLatestBlossom::AptLatestBlossom() :
    Blossom() {}

/**
 * @brief AptLatestBlossom::initTask
 * @return
 */
void
AptLatestBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.inputValues.contains("names") == false)
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
 * @brief AptLatestBlossom::preCheck
 * @return
 */
void
AptLatestBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief AptInstallBlossom::runTask
 * @return
 */
void
AptLatestBlossom::runTask(BlossomItem &blossomItem)
{
    std::string appendedList = "";
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        appendedList += m_packageNames.at(i) + " ";
    }

    const std::string programm = "sudo apt-get install -y " + appendedList;
    runSyncProcess(blossomItem, programm);
}

/**
 * @brief AptInstallBlossom::postCheck
 * @return
 */
void
AptLatestBlossom::postCheck(BlossomItem &blossomItem)
{
    m_packageNames = getAbsendPackages(blossomItem, m_packageNames);
    if(m_packageNames.size() > 0)
    {
        std::string output = "couldn't install following packages: \n";
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
 * @brief AptInstallBlossom::closeTask
 * @return
 */
void
AptLatestBlossom::closeTask(BlossomItem &blossomItem)
{
    m_packageNames.clear();
    blossomItem.success = true;
}

}
