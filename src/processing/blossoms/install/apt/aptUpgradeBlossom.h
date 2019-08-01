/**
 *  @file    aptUpgradeBlosson.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APTUPGRADEBLOSSOM_H
#define APTUPGRADEBLOSSOM_H

#include <processing/blossoms/install/apt/aptBlossom.h>

namespace SakuraTree
{

class AptUpgradeBlossom : public AptBlossom
{

public:
    AptUpgradeBlossom();

protected:
    void initTask(BlossomData *blossomData);
    void preCheck(BlossomData *blossomData);
    void runTask(BlossomData *blossomData);
    void postCheck(BlossomData *blossomData);
    void closeTask(BlossomData *blossomData);

private:
    std::string m_action = "";
};

}

#endif // APTUPGRADEBLOSSOM_H
