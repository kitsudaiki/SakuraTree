/**
 *  @file    aptPresentBlossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APTPRESENTBLOSSOM_H
#define APTPRESENTBLOSSOM_H

#include <processing/blossoms/install/apt/aptBlossom.h>

namespace SakuraSeed
{

class AptPresentBlossom : public AptBlossom
{

public:
    AptPresentBlossom();

protected:
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);
};

}

#endif // APTPRESENTBLOSSOM_H
