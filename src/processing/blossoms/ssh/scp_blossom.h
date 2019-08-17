/**
 *  @file    scp_blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SCP_BLOSSOM_H
#define SCP_BLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraTree
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

#endif // SCP_BLOSSOM_H
