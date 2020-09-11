/**
 * @file        text_blossoms.h
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

#ifndef TEXT_BLOSSOMS_H
#define TEXT_BLOSSOMS_H

#include <common.h>

//==================================================================================================
// TextAppendBlossom
//==================================================================================================
class TextAppendBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    TextAppendBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

//==================================================================================================
// TextReadBlossom
//==================================================================================================
class TextReadBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    TextReadBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

//==================================================================================================
// TextReplaceBlossom
//==================================================================================================
class TextReplaceBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    TextReplaceBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

//==================================================================================================
// TextWriteBlossom
//==================================================================================================
class TextWriteBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    TextWriteBlossom();

protected:
    bool runTask(BlossomLeaf &blossomLeaf, std::string &errorMessage);
};

#endif // TEXT_BLOSSOMS_H
