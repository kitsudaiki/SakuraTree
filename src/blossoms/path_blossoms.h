/**
 * @file        path_blossoms.h
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

#ifndef PATH_BLOSSOMS_H
#define PATH_BLOSSOMS_H

#include <common.h>

//==================================================================================================
// PathChmodBlossom
//==================================================================================================
class PathChmodBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PathChmodBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// PathChownBlossom
//==================================================================================================
class PathChownBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PathChownBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// PathCopyBlossom
//==================================================================================================
class PathCopyBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PathCopyBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// PathDeleteBlossom
//==================================================================================================
class PathDeleteBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PathDeleteBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// PathRenameBlossom
//==================================================================================================
class PathRenameBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PathRenameBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

#endif // PATH_BLOSSOMS_H
