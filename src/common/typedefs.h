/**
 * @file        typedefs.h
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
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

typedef std::pair<bool, std::string> Result;

namespace SakuraTree {
class SakuraBranch;
}
typedef std::map<std::string, SakuraTree::SakuraBranch*> BranchMap;


#endif // TYPEDEFS_H
