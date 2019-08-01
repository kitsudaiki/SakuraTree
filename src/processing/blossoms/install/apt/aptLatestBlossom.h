/**
 *  @file    aptLatestBlossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APTLATESTBLOSSOM_H
#define APTLATESTBLOSSOM_H

#include <processing/blossoms/install/apt/aptBlossom.h>

namespace SakuraTree
{

class AptLatestBlossom : public AptBlossom
{

public:
    AptLatestBlossom();

protected:
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);
};

}

#endif // APTLATESTBLOSSOM_H
