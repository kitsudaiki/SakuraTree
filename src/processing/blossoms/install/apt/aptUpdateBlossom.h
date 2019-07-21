/**
 *  @file    aptUpdateBlossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APTUPDATEBLOSSOM_H
#define APTUPDATEBLOSSOM_H

#include <processing/blossoms/install/apt/aptBlossom.h>

namespace SakuraSeed
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

#endif // APTUPDATEBLOSSOM_H
