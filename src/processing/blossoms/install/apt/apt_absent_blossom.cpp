/**
 *  @file    apt_absent_blossom.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "apt_absent_blossom.h"

namespace SakuraTree
{

AptAbsentBlossom::AptAbsentBlossom() :
    AptBlossom() {}

/**
 * initTask
 */
void
AptAbsentBlossom::initTask(BlossomData *blossomData)
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
 * preCheck
 */
void
AptAbsentBlossom::preCheck(BlossomData *blossomData)
{
    m_packageNames = getInstalledPackages(blossomData, m_packageNames);
    if(m_packageNames.size() == 0) {
        blossomData->skip = true;
    }

    blossomData->success = true;
}

/**
 * runTask
 */
void
AptAbsentBlossom::runTask(BlossomData *blossomData)
{
    std::string appendedList = "";
    for(uint32_t i = 0; i < m_packageNames.size(); i++)
    {
        appendedList += m_packageNames.at(i) + " ";
    }

    std::string programm = "sudo apt-get remove -y " + appendedList;
    runSyncProcess(blossomData, programm);
}

/**
 * postCheck
 */
void
AptAbsentBlossom::postCheck(BlossomData *blossomData)
{
    m_packageNames = getInstalledPackages(blossomData, m_packageNames);
    if(m_packageNames.size() > 0)
    {
        std::string output = "couldn't remove following packages: \n";
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
 * closeTask
 */
void
AptAbsentBlossom::closeTask(BlossomData *blossomData)
{
    m_packageNames.clear();
    blossomData->success = true;
}

}
