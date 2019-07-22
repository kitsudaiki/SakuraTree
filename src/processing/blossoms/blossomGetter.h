/**
 *  @file    blossomGetter.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef BLOSSOMGETTER_H
#define BLOSSOMGETTER_H

#include <common.h>

namespace SakuraSeed
{
class Blossom;

Blossom* getBlossom(const std::string type,
                    const std::string subType);

}

#endif // BLOSSOMGETTER_H
