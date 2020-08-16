/**
 * @file        config.h
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

#ifndef CONFIG_H
#define CONFIG_H

#include <common.h>
#include <libKitsunemimiConfig/config_handler.h>

void
registerConfigs()
{
    assert(REGISTER_BOOL_CONFIG("DEFAULT", "debug", false));
    assert(REGISTER_INT_CONFIG("server", "server_port", 1337));
    assert(REGISTER_STRING_CONFIG("server", "server_address", "127.0.0.1"));
}

#endif // CONFIG_H
