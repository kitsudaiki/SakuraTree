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
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);
};

}

#endif // APT_ABSENT_BLOSSOM_H
