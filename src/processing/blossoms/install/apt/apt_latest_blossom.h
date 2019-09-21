/**
 *  @file    apt_latest_blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APT_LATEST_BLOSSOM_H
#define APT_LATEST_BLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraTree
{

class AptLatestBlossom : public Blossom
{

public:
    AptLatestBlossom();

protected:
    void initTask(BlossomItem &blossomItem);
    void preCheck(BlossomItem &blossomItem);
    void runTask(BlossomItem &blossomItem);
    void postCheck(BlossomItem &blossomItem);
    void closeTask(BlossomItem &blossomItem);

private:
    std::vector<std::string> m_packageNames;
};

}

#endif // APT_LATEST_BLOSSOM_H
