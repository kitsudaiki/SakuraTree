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
#include <data_structure/data_items.h>

using Kitsune::Common::DataObject;

namespace SakuraTree
{

struct BlossomData
{
    std::string name = "";
    std::string type = "";

    DataObject* items;
    DataObject* settings;

    BlossomStates resultState = UNDEFINED_STATE;
    std::vector<std::string> nameHirarchie;

    bool skip = false;
    bool success = true;
    int execState = 0;
    std::string outputMessage = "";
};

class Blossom
{
public:
    Blossom();
    virtual ~Blossom();

    void growBlossom(BlossomData *blossomData);

    bool runSyncProcess(BlossomData *blossomData,
                        std::string command);
    bool runSyncProcess(BlossomData *blossomData,
                        const std::string &programm,
                        const std::vector<std::string> &args);

protected:
    virtual void initTask(BlossomData *blossomData) = 0;
    virtual void preCheck(BlossomData *blossomData) = 0;
    virtual void runTask(BlossomData *blossomData) = 0;
    virtual void postCheck(BlossomData *blossomData) = 0;
    virtual void closeTask(BlossomData *blossomData) = 0;
};

}

#endif // BLOSSOM_H
