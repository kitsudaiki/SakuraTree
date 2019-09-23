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

#include <processing/blossoms/blossom.h>

namespace SakuraTree
{

class AptUpgradeBlossom : public Blossom
{

public:
    AptUpgradeBlossom();

protected:
    void initTask(BlossomItem &blossomItem);
    void preCheck(BlossomItem &blossomItem);
    void runTask(BlossomItem &blossomItem);
    void postCheck(BlossomItem &blossomItem);
    void closeTask(BlossomItem &blossomItem);

private:
    std::string m_action = "";
    std::vector<std::string> m_packageNames;
};

}

#endif // APT_UPGRADE_BLOSSOM_H
