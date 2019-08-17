/**
 *  @file    blossom_getter.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef BLOSSOM_GETTER_H
#define BLOSSOM_GETTER_H

#include <common.h>

namespace SakuraTree
{
class Blossom;

Blossom* getBlossom(const std::string type,
                    const std::string subType);

}

#endif // BLOSSOM_GETTER_H
