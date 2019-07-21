/**
 *  @file    scpBlossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SCPBLOSSOM_H
#define SCPBLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraSeed
{

class ScpBlossom : public Blossom
{
public:
    ScpBlossom();

protected:
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);
};

}

#endif // SCPBLOSSOM_H
