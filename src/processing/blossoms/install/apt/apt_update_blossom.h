/**
 *  @file    apt_update_blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APT_UPDATE_BLOSSOM_H
#define APT_UPDATE_BLOSSOM_H

#include <processing/blossoms/install/apt/apt_blossom.h>

namespace SakuraTree
{

class AptUdateBlossom : public AptBlossom
{

public:
    AptUdateBlossom();

protected:
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);
};

}

#endif // APT_UPDATE_BLOSSOM_H
