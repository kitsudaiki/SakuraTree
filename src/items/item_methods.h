/**
 * @file        common_methods.h
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

#ifndef COMMON_METHODS_H
#define COMMON_METHODS_H

#include <common.h>

using Kitsunemimi::Common::DataMap;
namespace SakuraTree
{
struct BlossomItem;

const std::pair<bool, std::string> fillItems(DataMap &items,
                                             DataMap &insertValues);

void overrideItems(DataMap &original,
                   JsonItem &override);
void overrideItems(DataMap &original,
                   DataMap &override);

const std::vector<std::string> checkItems(DataMap &items);

const std::string convertBlossomOutput(const BlossomItem &blossom);

}

#endif // COMMON_METHODS_H
