/**
 * @file        apt_upgrade_blossom.h
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

#ifndef APT_UPGRADE_BLOSSOM_H
#define APT_UPGRADE_BLOSSOM_H

#include <libKitsunemimiSakuraLang/blossom.h>

class AptUpgradeBlossom
        : public Kitsunemimi::Sakura::Blossom
{

public:
    AptUpgradeBlossom();

protected:
    void runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem);
};

#endif // APT_UPGRADE_BLOSSOM_H
