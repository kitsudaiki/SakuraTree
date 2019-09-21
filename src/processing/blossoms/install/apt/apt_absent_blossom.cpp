/**
 *  @file    apt_absent_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_absent_blossom.h"
#include <processing/blossoms/install/apt/apt_methods.h>
#include <processing/process_methods.h>

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
 * preCheck
 */
void
AptAbsentBlossom::preCheck(BlossomItem &blossomItem)
{
    m_packageNames = getInstalledPackages(blossomItem, m_packageNames);
    if(m_packageNames.size() == 0)
    {
        blossomItem.skip = true;
        return;
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
        blossomItem.outputMessage = output;
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
