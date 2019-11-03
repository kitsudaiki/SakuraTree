/**
 *  @file    blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef BLOSSOM_H
#define BLOSSOM_H

#include <common.h>
#include <items/sakura_items.h>

using Kitsunemimi::Common::DataMap;

namespace SakuraTree
{

class Blossom
{
public:
    Blossom();
    virtual ~Blossom();

    void growBlossom(BlossomItem &blossomItem);

protected:
    virtual void initTask(BlossomItem &blossomItem) = 0;
    virtual void preCheck(BlossomItem &blossomItem) = 0;
    virtual void runTask(BlossomItem &blossomItem) = 0;
    virtual void postCheck(BlossomItem &blossomItem) = 0;
    virtual void closeTask(BlossomItem &blossomItem) = 0;
};

}

#endif // BLOSSOM_H
