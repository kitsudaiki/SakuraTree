/**
 * @file        apt_blossoms.h
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

#ifndef APT_BLOSSOMS_H
#define APT_BLOSSOMS_H

#include <common.h>

//==================================================================================================
// AptAbsentBlossom
//==================================================================================================
class AptAbsentBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    AptAbsentBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// AptLatestBlossom
//==================================================================================================
class AptLatestBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    AptLatestBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// AptPresentBlossom
//==================================================================================================
class AptPresentBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    AptPresentBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// AptUdateBlossom
//==================================================================================================
class AptUdateBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    AptUdateBlossom();

protected:
    bool runTask(BlossomItem &, std::string &errorMessage);
};

//==================================================================================================
// AptUpgradeBlossom
//==================================================================================================
class AptUpgradeBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    AptUpgradeBlossom();

protected:
    bool runTask(BlossomItem &, std::string &errorMessage);
};

#endif // APT_BLOSSOMS_H
