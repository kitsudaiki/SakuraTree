/**
 *  @file    blossomTypeHandler.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef BLOSSOMTYPEHANDLER_H
#define BLOSSOMTYPEHANDLER_H

#include <common.h>

namespace SakuraSeed
{
class Blossom;

class BlossomTypeHandler
{
public:
    BlossomTypeHandler();

    void initBlossoms();
    Blossom* getBlossom(const std::string type,
                        const std::string subType);

private:
    std::map<std::string, std::map<std::string, Blossom*>> m_allBlossoms;
};

}

#endif // BLOSSOMTYPEHANDLER_H
