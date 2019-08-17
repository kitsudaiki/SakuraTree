/**
 *  @file    apt_upgrade_blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APT_UPGRADE_BLOSSOM_H
#define APT_UPGRADE_BLOSSOM_H

#include <processing/blossoms/install/apt/apt_blossom.h>

namespace SakuraTree
{

class AptUpgradeBlossom : public AptBlossom
{

public:
    AptUpgradeBlossom();

protected:
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);

private:
    std::string m_action = "";
};

}

#endif // APT_UPGRADE_BLOSSOM_H
