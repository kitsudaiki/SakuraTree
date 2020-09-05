/**
 * @file        network_callbacks.h
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

#ifndef SAKURA_NETWORK_CALLBACKS_H
#define SAKURA_NETWORK_CALLBACKS_H

#include "messages.h"
#include "sakura_network.h"
#include "host_handler.h"

#include <libKitsunemimiSakuraNetwork/session.h>
#include <libKitsunemimiSakuraNetwork/session_controller.h>

#include <libKitsunemimiCommon/buffer/data_buffer.h>

#include <libKitsunemimiPersistence/logger/logger.h>


using Kitsunemimi::Sakura::Session;
using Kitsunemimi::Sakura::SessionController;

void streamDataCallback(void* target,
                        Session*,
                        const void* data,
                        const uint64_t dataSize)
{
    if(dataSize == 0) {
        return;
    }
    SakuraNetwork* sakuraHostHandler = static_cast<SakuraNetwork*>(target);
    const uint8_t type = static_cast<const uint8_t*>(data)[0];

    //==============================================================================================
    if(type == SAKURA_BLOSSOM_OUTPUT_TYPE)
    {
        const Sakura_Blossom_Output_Message* heander =
                static_cast<const Sakura_Blossom_Output_Message*>(data);

        const char* message = static_cast<const char*>(data);

        const std::string hostName(heander->hostName, heander->hostNameSize);
        const std::string receivedMessage(&message[sizeof (Sakura_Blossom_Output_Message)],
                                          heander->outputSize);

        sakuraHostHandler->m_incomingBlossomOutput(sakuraHostHandler->m_target,
                                                   hostName,
                                                   receivedMessage);
    }
    //==============================================================================================
}

/**
 * @brief dataCallback
 * @param target
 * @param session
 * @param isStream
 * @param data
 * @param dataSize
 */
void
standaloneMessageCallback(void* target,
                          Session* session,
                          const uint64_t blockerId,
                          Kitsunemimi::DataBuffer* data)
{
    if(data->bufferPosition == 0) {
        return;
    }
    SakuraNetwork* sakuraHostHandler = static_cast<SakuraNetwork*>(target);
    const uint8_t type = static_cast<const uint8_t*>(data->data)[0];

    //==============================================================================================
    if(type == SAKURA_INIT_TYPE)
    {
        const Sakura_Init_Message* message = static_cast<const Sakura_Init_Message*>(data->data);
        const std::string address(message->address, message->addressSize);
        const std::string hostName(message->hostName, message->hostNameSize);

        LOG_DEBUG("SAKURA_INIT_TYPE: \n"
                  "    address: " + address + "\n"
                  "    hostName: " + hostName);

        sakuraHostHandler->m_hostHandler->updateHost(hostName, session);
        sakuraHostHandler->m_incomingHost(sakuraHostHandler->m_target, hostName);
    }
    //==============================================================================================
    if(type == SAKURA_OBJECT_TRANSFER_TYPE)
    {
        const Sakura_Object_Transfer_Message* heander =
                static_cast<const Sakura_Object_Transfer_Message*>(data->data);

        const char* message = static_cast<const char*>(data->data);

        const uint8_t objectType = heander->objectType;
        const std::string path(heander->relativcePath, heander->relativePathSize);

        LOG_DEBUG("SAKURA_OBJECT_TRANSFER_TYPE: \n"
                  "    path: " + path);
        sakuraHostHandler->m_incomingData(sakuraHostHandler->m_target,
                                          objectType,
                                          path,
                                          &message[sizeof (Sakura_Object_Transfer_Message)],
                                          heander->contentSize);

        Sakura_Object_Response_Message response;
        session->sendResponse(&response, sizeof(Sakura_Object_Response_Message), blockerId);
    }
    //==============================================================================================
    if(type == SAKURA_SEED_TRIGGER_TYPE)
    {
        LOG_DEBUG("");

        const Sakura_Seed_Trigger_Message* heander =
                static_cast<const Sakura_Seed_Trigger_Message*>(data->data);

        const char* message = static_cast<const char*>(data->data);

        const std::string treeId(heander->treeId, heander->treeIdSize);
        const std::string values(&message[sizeof (Sakura_Seed_Trigger_Message)],
                                 heander->valuesSize);

        LOG_DEBUG("SAKURA_SEED_TRIGGER_TYPE: \n"
                  "    treeId: " + treeId + "\n"
                  "    values: " + values);

        sakuraHostHandler->m_incomingSeedTrigger(sakuraHostHandler->m_target,
                                                 treeId,
                                                 values,
                                                 session,
                                                 blockerId);

        Sakura_Seed_Response_Message response;
        session->sendResponse(&response, sizeof(Sakura_Seed_Response_Message), blockerId);
    }
    //==============================================================================================

    delete data;
}

/**
 * @brief errorCallback
 * @param errorMsg
 */
void
errorCallback(void*,
              Session* session,
              const uint8_t,
              const std::string errorMsg)
{
    LOG_ERROR("Error in libKitsuneSakuraNetwork: " + errorMsg);
    session->closeSession();
}

/**
 * @brief sessionCallback
 * @param target
 * @param isInit
 * @param session
 */
void
sessionCallback(void* target,
                bool isInit,
                Session* session,
                const std::string)
{
    if(isInit)
    {
        SakuraNetwork* sakuraHostHandler = static_cast<SakuraNetwork*>(target);

        Sakura_Init_Message message;
        memcpy(message.address,
               sakuraHostHandler->m_address.c_str(),
               sakuraHostHandler->m_address.size());
        message.addressSize = static_cast<uint32_t>(sakuraHostHandler->m_address.size());
        memcpy(message.hostName,
               sakuraHostHandler->m_hostName.c_str(),
               sakuraHostHandler->m_hostName.size());
        message.hostNameSize = static_cast<uint32_t>(sakuraHostHandler->m_hostName.size());

        session->sendStandaloneData(&message, sizeof (Sakura_Init_Message));
    }
}

#endif // SAKURA_NETWORK_CALLBACKS_H
