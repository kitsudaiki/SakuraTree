/**
 * @file        messages.h
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

#ifndef SAKUSA_MESSAGES_H
#define SAKUSA_MESSAGES_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

enum types
{
    UNDEFINED_TYPE = 0,
    SAKURA_INIT_TYPE = 1,
    SAKURA_OBJECT_TRANSFER_TYPE = 2,
    SAKURA_OBJECT_RESPONSE_TYPE = 3,
    SAKURA_SEED_TRIGGER_TYPE = 4,
    SAKURA_SEED_RESPONSE_TYPE = 5,
    SAKURA_BLOSSOM_OUTPUT_TYPE = 6,
};

enum objectType
{
    UNDEFINED_OBJECT_TYPE = 0,
    TREE_OBJECT_TYPE = 1,
    RESSOURCE_OBJECT_TYPE = 2,
    FILE_OBJECT_TYPE = 3,
    TEMPLATE_OBJECT_TYPE = 4,
};

struct Sakura_Init_Message
{
    uint8_t type = SAKURA_INIT_TYPE;
    uint32_t addressSize = 0;
    char address[50];
    uint32_t hostNameSize = 0;
    char hostName[50];
};

struct Sakura_Object_Transfer_Message
{
    uint8_t type = SAKURA_OBJECT_TRANSFER_TYPE;
    uint8_t objectType = UNDEFINED_OBJECT_TYPE;
    uint32_t relativePathSize = 0;
    char relativcePath[1024];
    uint32_t contentSize = 0;
};

struct Sakura_Object_Response_Message
{
    uint8_t type = SAKURA_OBJECT_RESPONSE_TYPE;
};

struct Sakura_Seed_Trigger_Message
{
    uint8_t type = SAKURA_SEED_TRIGGER_TYPE;
    uint32_t treeIdSize = 0;
    char treeId[50];
    uint32_t valuesSize = 0;
};

struct Sakura_Seed_Response_Message
{
    uint8_t type = SAKURA_SEED_RESPONSE_TYPE;
};

struct Sakura_Blossom_Output_Message
{
    uint8_t type = SAKURA_BLOSSOM_OUTPUT_TYPE;
    uint32_t hostNameSize = 0;
    char hostName[50];
    uint32_t outputSize = 0;
};

#endif // SAKUSA_MESSAGES_H
