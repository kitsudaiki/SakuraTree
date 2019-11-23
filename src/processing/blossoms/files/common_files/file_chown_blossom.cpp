/**
 * @file        file_chown_blossom.cpp
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

#include "file_chown_blossom.h"
#include <processing/blossoms/files/file_methods.h>

namespace SakuraTree
{

FileChownBlossom::FileChownBlossom() :
    Blossom() {}

/**
 * @brief initTask
 */
void
FileChownBlossom::initTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief preCheck
 */
void
FileChownBlossom::preCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief runTask
 */
void
FileChownBlossom::runTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief postCheck
 */
void
FileChownBlossom::postCheck(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

/**
 * @brief closeTask
 */
void
FileChownBlossom::closeTask(BlossomItem &blossomItem)
{
    blossomItem.success = true;
}

}