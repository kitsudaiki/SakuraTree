/**
 *  @file    aptAbsentBlossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APTABSENTBLOSSOM_H
#define APTABSENTBLOSSOM_H

#include <processing/blossoms/install/apt/aptBlossom.h>

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

#endif // APTABSENTBLOSSOM_H
