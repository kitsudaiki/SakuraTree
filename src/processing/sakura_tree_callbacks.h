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
#include <tree_handler.h>

#include <libKitsunemimiProjectNetwork/session.h>
#include <libKitsunemimiSakuraParser/sakura_parsing.h>
#include <libKitsunemimiCommon/buffer/data_buffer.h>
#include <libKitsunemimiPersistence/logger/logger.h>

namespace SakuraTree
{

enum objectType
{
    UNDEFINED_OBJECT_TYPE = 0,
    TREE_OBJECT_TYPE = 1,
    FILE_OBJECT_TYPE = 2,
    TEMPLATE_OBJECT_TYPE = 3,
};

void objectTransferCallback(void* target,
                            const uint8_t objectType,
                            const std::string path,
                            const void* data,
                            const uint64_t dataSize)
{
    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);

    if(objectType == TREE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive tree-object with relative path " + path);

        Kitsunemimi::Sakura::SakuraParsing parser;
        const std::string content(static_cast<const char*>(data), dataSize);

        std::string errorMessage = "";
        SakuraItem* parsedItem = parser.parseString(content, errorMessage)->copy();

        TreeItem* parsedTree = dynamic_cast<TreeItem*>(parsedItem);
        parsedTree->relativePath = path;
        parsedTree->rootPath = "/tmp";
        parsedTree->unparsedConent = content;

        rootClass->m_treeHandler->m_garden.trees.insert(std::make_pair(path, parsedTree));
    }

    if(objectType == FILE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive file-object with relative path " + path);

        const uint32_t numberOfBlocks = static_cast<uint32_t>((dataSize / 4096) + 1);
        Kitsunemimi::DataBuffer* buffer = new Kitsunemimi::DataBuffer(numberOfBlocks);
        memcpy(buffer->data, data, dataSize);
        buffer->bufferPosition = dataSize;

        rootClass->m_treeHandler->m_garden.files.insert(std::make_pair(path, buffer));
    }

    if(objectType == TEMPLATE_OBJECT_TYPE)
    {
        LOG_DEBUG("receive template-object with relative path " + path);

        const std::string content(static_cast<const char*>(data), dataSize);
        rootClass->m_treeHandler->m_garden.templates.insert(std::make_pair(path, content));
    }
}

void seedTriggerCallback(void* target,
                         const std::string path,
                         const std::string values,
                         Kitsunemimi::Project::Session* session,
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
}

}

#endif // SAKURA_TREE_CALLBACKS_H
