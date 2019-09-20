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
#include <common_items/data_items.h>
#include <items/sakura_items.h>

using Kitsune::Common::DataMap;

namespace SakuraTree
{

class Blossom
{
public:
    Blossom();
    virtual ~Blossom();

    void growBlossom(BlossomItem &blossomItem);

    bool runSyncProcess(BlossomItem &blossomItem,
                        std::string command);
    bool runSyncProcess(BlossomItem &blossomItem,
                        const std::string &programm,
                        const std::vector<std::string> &args);

protected:
    virtual void initTask(BlossomItem &blossomItem) = 0;
    virtual void preCheck(BlossomItem &blossomItem) = 0;
    virtual void runTask(BlossomItem &blossomItem) = 0;
    virtual void postCheck(BlossomItem &blossomItem) = 0;
    virtual void closeTask(BlossomItem &blossomItem) = 0;
};

}

#endif // BLOSSOM_H
