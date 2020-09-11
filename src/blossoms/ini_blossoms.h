/**
 * @file        ini_blossoms.h
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

#ifndef INI_BLOSSOMS_H
#define INI_BLOSSOMS_H

#include <common.h>

//==================================================================================================
// IniDeleteEntryBlossom
//==================================================================================================
class IniDeleteEntryBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    IniDeleteEntryBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

//==================================================================================================
// IniReadEntryBlossom
//==================================================================================================
class IniReadEntryBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    IniReadEntryBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

//==================================================================================================
// IniSetEntryBlossom
//==================================================================================================
class IniSetEntryBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    IniSetEntryBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

#endif // INI_BLOSSOMS_H
