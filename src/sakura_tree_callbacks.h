/**
 * @file        sakura_tree_callbacks.h
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

#ifndef SAKURA_TREE_CALLBACKS_H
#define SAKURA_TREE_CALLBACKS_H

#include <sakura_root.h>

#include <libKitsunemimiSakuraNetwork/session.h>
#include <libKitsunemimiSakuraLang/sakura_parsing.h>
#include <libKitsunemimiCommon/buffer/data_buffer.h>
#include <libKitsunemimiPersistence/logger/logger.h>

namespace Kitsunemimi
{
namespace Sakura
{

enum objectType
{
    UNDEFINED_OBJECT_TYPE = 0,
    TREE_OBJECT_TYPE = 1,
    RESSOURCE_OBJECT_TYPE = 2,
    FILE_OBJECT_TYPE = 3,
    TEMPLATE_OBJECT_TYPE = 4,
};

/*void objectTransferCallback(void* target,
                            const uint8_t objectType,
                            const std::string path,
                            const void* data,
                            const uint64_t dataSize)
{
    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
    std::string errorMessage = "";

    //----------------------------------------------------------------------------------------------
    if(objectType == TREE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive tree-object with relative path " + path);

        Kitsunemimi::Sakura::SakuraParsing parser;
        const std::string content(static_cast<const char*>(data), dataSize);

        parser.parseTreeString(*rootClass->m_currentGarden,
                               path,
                               content,
                               errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(objectType == RESSOURCE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive ressource-object with id " + path);

        Kitsunemimi::Sakura::SakuraParsing parser;
        const std::string content(static_cast<const char*>(data), dataSize);

        parser.parseRessourceString(*rootClass->m_currentGarden,
                                    content,
                                    errorMessage);
    }
    //----------------------------------------------------------------------------------------------
    if(objectType == FILE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive file-object with relative path " + path);

        const uint32_t numberOfBlocks = static_cast<uint32_t>((dataSize / 4096) + 1);
        Kitsunemimi::DataBuffer* buffer = new Kitsunemimi::DataBuffer(numberOfBlocks);
        memcpy(buffer->data, data, dataSize);
        buffer->bufferPosition = dataSize;

        rootClass->m_currentGarden->files.insert(std::make_pair(path, buffer));
    }
    //----------------------------------------------------------------------------------------------
    if(objectType == TEMPLATE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive template-object with relative path " + path);

        const std::string content(static_cast<const char*>(data), dataSize);
        rootClass->m_currentGarden->templates.insert(std::make_pair(path, content));
    }
    //----------------------------------------------------------------------------------------------
}

void seedTriggerCallback(void* target,
                         const std::string path,
                         const std::string values,
                         Kitsunemimi::Sakura::Session* session,
                         const uint64_t blockerId)
{
    LOG_DEBUG("receive seed-trigger with relative path " + path);

    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
    rootClass->startSubtreeProcess(path, values, session, blockerId);
}

void blossomOutputCallback(void* target,
                           const std::string ,
                           const std::string output)
{
    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
    rootClass->printOutput(output);
}


void sessionCallback(void* ,
                     const std::string)
{
    //SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
}*/

}
}

#endif // SAKURA_TREE_CALLBACKS_H
