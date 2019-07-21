/**
 *  @file    typedefs.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <chrono>
#include <map>
#include <string>

typedef std::chrono::microseconds chronoMicroSec;
typedef std::chrono::nanoseconds chronoNanoSec;
typedef std::chrono::seconds chronoSec;
typedef std::chrono::high_resolution_clock::time_point chronoTimePoint;
typedef std::chrono::high_resolution_clock chronoClock;


namespace SakuraSeed {
class SakuraBranch;
}
typedef std::map<std::string, SakuraSeed::SakuraBranch*> BranchMap;


#endif // TYPEDEFS_H
