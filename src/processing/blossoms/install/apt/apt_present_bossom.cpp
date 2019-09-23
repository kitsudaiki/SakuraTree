/**
 *  @file    apt_present_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_present_blossom.h"
#include <processing/blossoms/install/apt/apt_methods.h>
#include <processing/process_methods.h>

namespace SakuraTree
{

/**
 * @brief AptInstallBlossom::AptInstallBlossom
 * @param content
 */
AptPresentBlossom::AptPresentBlossom() :
    Blossom() {}

/**
 * @brief AptInstallBlossom::initTask
 * @return
 */
void
AptPresentBlossom::initTask(BlossomItem &blossomItem)
{
    if(blossomItem.values.contains("names") == false)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "no packages to defined";
        return;
    }

    fillPackageNames(blossomItem, m_packageNames);

    if(m_packageNames.size() == 0)
    {
        blossomItem.success = false;
        blossomItem.outputMessage = "no packages to defined";
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief AptInstallBlossom::preCheck
 * @return
 */
void
AptPresentBlossom::preCheck(BlossomItem &blossomItem)
{
    m_packageNames = getAbsendPackages(blossomItem, m_packageNames);

    if(m_packageNames.size() == 0) {
        blossomItem.skip = true;
    }

    blossomItem.success = true;
}

/**
 * @brief AptInstallBlossom::runTask
 * @return
 */
void
AptPresentBlossom::runTask(BlossomItem &blossomItem)
{
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        std::string programm = "sudo apt-get install -y " + m_packageNames.at(i);
        runSyncProcess(blossomItem, programm);
    }

    blossomItem.success = true;
}

/**
 * @brief AptInstallBlossom::postCheck
 * @return
 */
void
AptPresentBlossom::postCheck(BlossomItem &blossomItem)
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
        blossomItem.outputMessage = output;
        return;
    }

    blossomItem.success = true;
}

/**
 * @brief AptInstallBlossom::closeTask
 * @return
 */
void
AptPresentBlossom::closeTask(BlossomItem &blossomItem)
{
    m_packageNames.clear();
    blossomItem.success = true;
}

}
