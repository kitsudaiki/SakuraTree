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
AptLatestBlossom::initTask(BlossomData *blossomData)
{
    if(blossomData->items->contains("names") == false)
    {
        blossomData->success = false;
        blossomData->outputMessage = "no packages to defined";
    }

    fillPackageNames(blossomData);

    if(m_packageNames.size() == 0)
    {
        blossomData->success = false;
        blossomData->outputMessage = "no packages to defined";
    }

    blossomData->success = true;
}

/**
 * @brief AptLatestBlossom::preCheck
 * @return
 */
void
AptLatestBlossom::preCheck(BlossomData *blossomData)
{
    blossomData->success = true;
}

/**
 * @brief AptInstallBlossom::runTask
 * @return
 */
void
AptLatestBlossom::runTask(BlossomData *blossomData)
{
    std::string appendedList = "";
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        appendedList += m_packageNames.at(i) + " ";
    }

    std::string programm = "sudo apt-get install -y " + appendedList;
    runSyncProcess(blossomData, programm);
}

/**
 * @brief AptInstallBlossom::postCheck
 * @return
 */
void
AptLatestBlossom::postCheck(BlossomData *blossomData)
{
    m_packageNames = getAbsendPackages(blossomData, m_packageNames);
    if(m_packageNames.size() > 0)
    {
        std::string output = "couldn't install following packages: \n";
        for(uint32_t i = 0; i < m_packageNames.size(); i++)
        {
            output += m_packageNames.at(i) + "\n";
        }

        blossomData->success = false;
        blossomData->outputMessage = output;
    }

    blossomData->success = true;
}

/**
 * @brief AptInstallBlossom::closeTask
 * @return
 */
void
AptLatestBlossom::closeTask(BlossomData *blossomData)
{
    m_packageNames.clear();
    blossomData->success = true;
}

}
