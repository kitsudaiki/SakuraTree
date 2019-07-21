/**
 *  @file    aptPresentBlossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "aptPresentBlossom.h"

namespace SakuraSeed
{

/**
 * @brief AptInstallBlossom::AptInstallBlossom
 * @param content
 */
AptPresentBlossom::AptPresentBlossom() :
    AptBlossom() {}

/**
 * @brief AptInstallBlossom::initTask
 * @return
 */
void
AptPresentBlossom::initTask(BlossomData* blossomData)
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
 * @brief AptInstallBlossom::preCheck
 * @return
 */
void
AptPresentBlossom::preCheck(BlossomData *blossomData)
{
    m_packageNames = getAbsendPackages(blossomData, m_packageNames);

    if(m_packageNames.size() == 0) {
        blossomData->skip = true;
    }

    blossomData->success = true;
}

/**
 * @brief AptInstallBlossom::runTask
 * @return
 */
void
AptPresentBlossom::runTask(BlossomData *blossomData)
{
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        std::string programm = "sudo apt-get install -y " + m_packageNames.at(i);
        runSyncProcess(blossomData, programm);
    }

    blossomData->success = true;
}

/**
 * @brief AptInstallBlossom::postCheck
 * @return
 */
void
AptPresentBlossom::postCheck(BlossomData *blossomData)
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
AptPresentBlossom::closeTask(BlossomData *blossomData)
{
    m_packageNames.clear();
    blossomData->success = true;
}

}
