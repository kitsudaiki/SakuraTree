/**
 * @file        sakura_network.cpp
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

#include "sakura_network.h"

#include <libKitsunemimiSakuraNetwork/session.h>
#include <libKitsunemimiSakuraNetwork/session_controller.h>

#include <libKitsunemimiSakuraLang/sakura_garden.h>
#include <libKitsunemimiSakuraLang/items/sakura_items.h>

#include <libKitsunemimiPersistence/logger/logger.h>
#include "network_callbacks.h"
#include "host_handler.h"


/**
 * @brief SakuraHostHandler::SakuraHostHandler
 */
SakuraNetwork::SakuraNetwork(void* target,
                             void (*incomingHost)(void*,
                                                  const std::string),
                             void (*incomingData)(void*,
                                                  const uint8_t,
                                                  const std::string,
                                                  const void*,
                                                  const uint64_t),
                             void (*incomingSeedTrigger)(void*,
                                                         const std::string,
                                                         const std::string,
                                                         Session*,
                                                         const uint64_t),
                             void (*incomingBlossomOutput)(void*,
                                                           const std::string,
                                                           const std::string))
{
    m_target = target;
    m_incomingHost = incomingHost;
    m_incomingData = incomingData;
    m_incomingSeedTrigger = incomingSeedTrigger;
    m_incomingBlossomOutput = incomingBlossomOutput;

    char hostname[100];
    gethostname(hostname, 100);
    m_hostName = std::string(hostname);

    m_hostHandler = new HostHandler();
    m_sessionController = new SessionController(this, &sessionCallback,
                                                this, &streamDataCallback,
                                                this, &standaloneMessageCallback,
                                                this, &errorCallback);
}

/**
 * @brief SakuraHostHandler::createServer
 * @param port
 * @return
 */
bool
SakuraNetwork::createServer(const uint16_t port)
{
    return m_sessionController->addTcpServer(port);
}

/**
 * @brief SakuraNetwork::registerHost
 * @param hostName
 * @param tags
 * @return
 */
bool
SakuraNetwork::registerHost(const std::string hostName,
                            const std::vector<std::string> tags)
{
    return m_hostHandler->addHost(hostName, tags);
}

/**
 * @brief SakuraNetwork::isHostReady
 * @param hostName
 * @return
 */
bool
SakuraNetwork::isHostReady(const std::string hostName)
{
    if(m_hostHandler->get(hostName) == nullptr) {
        return false;
    }

    return true;
}

/**
 * @brief SakuraNetwork::areAllHostsReady
 * @return
 */
bool
SakuraNetwork::areAllHostsReady()
{
    const std::vector<Kitsunemimi::Sakura::Session*> sessions = m_hostHandler->getAll();

    for(uint32_t i = 0; i < sessions.size(); i++)
    {
        if(sessions.at(i) == nullptr) {
            return false;
        }
    }

    return true;
}

/**
 * @brief SakuraNetwork::createClientConnection
 * @param address
 * @param port
 * @param type
 * @return
 */
bool
SakuraNetwork::createClientConnection(const std::string address,
                                      const uint16_t port,
                                      connectionType type)
{
    switch (type) {
        case TCP_CONNECTION:
            return m_sessionController->startTcpSession(address, port);
    }

    return false;
}

/**
 * @brief SakuraNetwork::closeAllSessions
 * @return
 */
bool
SakuraNetwork::closeAllSessions()
{
    m_sessionController->cloesAllServers();
    m_sessionController->closeAllSession();
    return true;
}

/**
 * @brief SakuraHostHandler::sendTreePlanToAll
 * @param treeId
 * @param tree
 * @return
 */
bool
SakuraNetwork::sendDataToAll(const Kitsunemimi::Sakura::SakuraGarden &garden)
{
    const std::vector<Kitsunemimi::Sakura::Session*> sessions = m_hostHandler->getAll();

    for(uint32_t i = 0; i < sessions.size(); i++)
    {
        const bool result = sendData(sessions.at(i), garden);
        if(result == false) {
            return result;
        }
    }

    return true;
}

/**
 * @brief SakuraHostHandler::sendTreePlanToHost
 * @param hostName
 * @param treeId
 * @param tree
 * @return
 */
bool
SakuraNetwork::sendDataToHost(const std::string &hostName,
                              const Kitsunemimi::Sakura::SakuraGarden &garden)
{
    Session* session = m_hostHandler->get(hostName);
    return sendData(session, garden);
}

/**
 * @brief SakuraHostHandler::sendTreePlan
 * @param session
 * @param treeId
 * @param tree
 * @return
 */
bool
SakuraNetwork::sendData(Kitsunemimi::Sakura::Session* session,
                        const Kitsunemimi::Sakura::SakuraGarden &garden)
{
    if(session == nullptr) {
        return false;
    }

    // send trees
    std::map<std::string, Kitsunemimi::Sakura::TreeItem*>::const_iterator itTrees;
    for(itTrees = garden.trees.begin();
        itTrees != garden.trees.end();
        itTrees++)
    {
        sendData(session,
                 TREE_OBJECT_TYPE,
                 itTrees->first,
                 itTrees->second->unparsedConent.c_str(),
                 itTrees->second->unparsedConent.size());
    }

    // send resources
    std::map<std::string, Kitsunemimi::Sakura::TreeItem*>::const_iterator itResources;
    for(itResources = garden.resources.begin();
        itResources != garden.resources.end();
        itResources++)
    {
        sendData(session,
                 RESSOURCE_OBJECT_TYPE,
                 itResources->first,
                 itResources->second->unparsedConent.c_str(),
                 itResources->second->unparsedConent.size());
    }

    // send files
    std::map<std::string, Kitsunemimi::DataBuffer*>::const_iterator itFiles;
    for(itFiles = garden.files.begin();
        itFiles != garden.files.end();
        itFiles++)
    {
        sendData(session,
                 FILE_OBJECT_TYPE,
                 itFiles->first,
                 itFiles->second->data,
                 itFiles->second->bufferPosition);
    }

    // send templates
    std::map<std::string, std::string>::const_iterator itTemplates;
    for(itTemplates = garden.templates.begin();
        itTemplates != garden.templates.end();
        itTemplates++)
    {
        sendData(session,
                 TEMPLATE_OBJECT_TYPE,
                 itTemplates->first,
                 itTemplates->second.c_str(),
                 itTemplates->second.size());
    }

    return true;
}

/**
 * @brief sendTreePlan
 * @param session
 * @param objectType
 * @param path
 * @param data
 * @param dataSize
 * @return
 */
bool
SakuraNetwork::sendData(Kitsunemimi::Sakura::Session* session,
                        const uint8_t objectType,
                        const std::string &path,
                        const void* data,
                        const uint64_t dataSize)
{
    // prepare message
    const uint64_t size = sizeof(Sakura_Object_Transfer_Message) + dataSize;
    uint8_t* message = new uint8_t[size];

    // create header
    Sakura_Object_Transfer_Message header;
    header.objectType = objectType;
    header.relativePathSize = static_cast<uint32_t>(path.size());
    header.contentSize = static_cast<uint32_t>(dataSize);

    // fill data
    memset(header.relativcePath, 0, 50);
    memcpy(header.relativcePath,
           path.c_str(),
           path.size());

    memcpy(message, &header, sizeof(Sakura_Object_Transfer_Message));
    memcpy(message + sizeof(Sakura_Object_Transfer_Message),
           data,
           dataSize);

    // send
    Kitsunemimi::DataBuffer* buffer = session->sendRequest(message, size, 0);
    if(buffer == nullptr) {
        return false;
    }

    delete buffer;

    return true;
}

/**
 * @brief SakuraHostHandler::triggerSeed
 * @param address
 * @param treeId
 * @param values
 * @return
 */
bool
SakuraNetwork::triggerSeedByHost(const std::string &hostName,
                                 const std::string &treeId,
                                 const std::string &values)
{
    Session* session = m_hostHandler->get(hostName);
    return triggerSeed(session, treeId, values);
}

/**
 * @brief SakuraHostHandler::triggerSeedByTag
 * @param tag
 * @param treeId
 * @param values
 * @return
 */
bool
SakuraNetwork::triggerSeedByTag(const std::string &tag,
                                const std::string &treeId,
                                const std::string &values)
{
    const std::vector<Kitsunemimi::Sakura::Session*> sessions = m_hostHandler->getByTag(tag);

    for(uint32_t i = 0; i < sessions.size(); i++)
    {
        const bool result = triggerSeed(sessions.at(i), treeId, values);
        if(result == false) {
            return result;
        }
    }

    return true;
}

/**
 * @brief SakuraHostHandler::triggerSeed
 * @param session
 * @param treeId
 * @param values
 * @return
 */
bool
SakuraNetwork::triggerSeed(Kitsunemimi::Sakura::Session* session,
                           const std::string &treeId,
                           const std::string &values)
{
    if(session == nullptr) {
        return false;
    }

    // prepare message
    const uint64_t size = sizeof(Sakura_Seed_Trigger_Message) + values.size();
    uint8_t* data = new uint8_t[size];

    // create header
    Sakura_Seed_Trigger_Message header;
    header.treeIdSize = static_cast<uint32_t>(treeId.size());
    header.valuesSize = static_cast<uint32_t>(values.size());

    // fill data
    memset(header.treeId, 0, 50);
    memcpy(header.treeId,
           treeId.c_str(),
           treeId.size());

    memcpy(data, &header, sizeof(Sakura_Seed_Trigger_Message));
    memcpy(data + sizeof(Sakura_Seed_Trigger_Message),
           values.c_str(),
           values.size());

    // send
    session->sendRequest(data, size, 0);

    return true;
}

/**
 * @brief SakuraHostHandler::sendBlossomOuput
 * @param address
 * @param hostName
 * @param output
 * @return
 */
bool
SakuraNetwork::sendBlossomOuput(const std::string &hostName,
                                const std::string &output)
{
    Session* session = m_hostHandler->get(hostName);
    if(session == nullptr) {
        return false;
    }

    // prepare message
    const uint64_t size = sizeof(Sakura_Blossom_Output_Message) + output.size();
    uint8_t* data = new uint8_t[size];

    // create header
    Sakura_Blossom_Output_Message header;
    header.hostNameSize = static_cast<uint32_t>(hostName.size());
    header.outputSize = static_cast<uint32_t>(output.size());

    // fill data
    memset(header.hostName, 0, 50);
    memcpy(header.hostName,
           hostName.c_str(),
           hostName.size());

    memcpy(data, &header, sizeof(Sakura_Blossom_Output_Message));
    memcpy(data + sizeof(Sakura_Blossom_Output_Message),
           output.c_str(),
           output.size());

    // send
    return session->sendStreamData(data, size);
}
