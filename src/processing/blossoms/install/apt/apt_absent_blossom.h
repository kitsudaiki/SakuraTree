/**
 *  @file    apt_absent_blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APT_ABSENT_BLOSSOM_H
#define APT_ABSENT_BLOSSOM_H

#include <processing/blossoms/install/apt/apt_blossom.h>

namespace SakuraTree
{

class AptAbsentBlossom : public AptBlossom
{

public:
    AptAbsentBlossom();

protected:
    void initTask(BlossomItem &blossomItem);
    void preCheck(BlossomItem &blossomItem);
    void runTask(BlossomItem &blossomItem);
    void postCheck(BlossomItem &blossomItem);
    void closeTask(BlossomItem &blossomItem);
};

}

#endif // APT_ABSENT_BLOSSOM_H
