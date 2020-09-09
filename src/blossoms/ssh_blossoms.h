/**
 * @file        ssh_blossoms.h
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

#ifndef SSH_BLOSSOMS_H
#define SSH_BLOSSOMS_H

#include <common.h>
#include <unistd.h>

//==================================================================================================
// SshCmdBlossom
//==================================================================================================
class SshCmdBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    SshCmdBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// SshCmdCreateFileBlossom
//==================================================================================================
class SshCmdCreateFileBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    SshCmdCreateFileBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};

//==================================================================================================
// SshScpBlossom
//==================================================================================================
class SshScpBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    SshScpBlossom();

protected:
    bool runTask(BlossomItem &blossomItem, std::string &errorMessage);
};


#endif // SSH_BLOSSOMS_H
