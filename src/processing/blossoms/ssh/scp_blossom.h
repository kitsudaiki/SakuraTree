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
    void initTask(BlossomItem *blossomItem);
    void preCheck(BlossomItem *blossomItem);
    void runTask(BlossomItem *blossomItem);
    void postCheck(BlossomItem *blossomItem);
    void closeTask(BlossomItem *blossomItem);
};

}

#endif // SCP_BLOSSOM_H
