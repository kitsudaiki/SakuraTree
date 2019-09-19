/**
 *  @file    apt_latest_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_latest_blossom.h"

namespace SakuraTree
{

AptLatestBlossom::AptLatestBlossom() :
    AptBlossom() {}

/**
 * @brief AptLatestBlossom::initTask
 * @return
 */
void
AptLatestBlossom::initTask(BlossomItem *blossomItem)
{
    if(blossomItem->values.contains("names") == false)
    {
        blossomItem->success = false;
        blossomItem->outputMessage = "no packages to defined";
    }

    fillPackageNames(blossomItem);

    if(m_packageNames.size() == 0)
    {
        blossomItem->success = false;
        blossomItem->outputMessage = "no packages to defined";
    }

    blossomItem->success = true;
}

/**
 * @brief AptLatestBlossom::preCheck
 * @return
 */
void
AptLatestBlossom::preCheck(BlossomItem *blossomItem)
{
    blossomItem->success = true;
}

/**
 * @brief AptInstallBlossom::runTask
 * @return
 */
void
AptLatestBlossom::runTask(BlossomItem *blossomItem)
{
    std::string appendedList = "";
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        appendedList += m_packageNames.at(i) + " ";
    }

    std::string programm = "sudo apt-get install -y " + appendedList;
    runSyncProcess(blossomItem, programm);
}

/**
 * @brief AptInstallBlossom::postCheck
 * @return
 */
void
AptLatestBlossom::postCheck(BlossomItem *blossomItem)
{
    m_packageNames = getAbsendPackages(blossomItem, m_packageNames);
    if(m_packageNames.size() > 0)
    {
        std::string output = "couldn't install following packages: \n";
        for(uint32_t i = 0; i < m_packageNames.size(); i++)
        {
            output += m_packageNames.at(i) + "\n";
        }

        blossomItem->success = false;
        blossomItem->outputMessage = output;
    }

    blossomItem->success = true;
}

/**
 * @brief AptInstallBlossom::closeTask
 * @return
 */
void
AptLatestBlossom::closeTask(BlossomItem *blossomItem)
{
    m_packageNames.clear();
    blossomItem->success = true;
}

}
