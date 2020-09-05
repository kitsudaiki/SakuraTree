/**
 * @file        sakura_network.h
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

#ifndef SAKURA_HOST_HANDLER_H
#define SAKURA_HOST_HANDLER_H

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <unistd.h>
#include <limits.h>

namespace Kitsunemimi
{
namespace Sakura {
class Session;
class SessionController;
}
namespace Sakura {
class SakuraGarden;
}
}

class HostHandler;

using Kitsunemimi::Sakura::Session;
using Kitsunemimi::Sakura::SessionController;

class SakuraNetwork
{

public:
    enum connectionType {
        TCP_CONNECTION = 0,
    };

    SakuraNetwork(void* sessionTarget,
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
                                                const std::string));

    bool createServer(const uint16_t port);
    bool registerHost(const std::string hostName,
                     const std::vector<std::string> tags);
    bool isHostReady(const std::string hostName);
    bool areAllHostsReady();
    bool createClientConnection(const std::string address,
                                const uint16_t port,
                                connectionType type=TCP_CONNECTION);
    bool closeAllSessions();

    // send data
    bool sendDataToAll(const Kitsunemimi::Sakura::SakuraGarden &garden);
    bool sendDataToHost(const std::string &hostName,
                        const Kitsunemimi::Sakura::SakuraGarden &garden);
    bool sendData(Kitsunemimi::Sakura::Session* session,
                  const Kitsunemimi::Sakura::SakuraGarden &garden);
    bool sendData(Kitsunemimi::Sakura::Session* session,
                  const uint8_t objectType,
                  const std::string &path,
                  const void* data,
                  const uint64_t dataSize);

    // seed-trigger
    bool triggerSeedByHost(const std::string &hostName,
                           const std::string &treeId,
                           const std::string &values);
    bool triggerSeedByTag(const std::string &tag,
                          const std::string &treeId,
                          const std::string &values);
    bool triggerSeed(Kitsunemimi::Sakura::Session* session,
                     const std::string &treeId,
                     const std::string &values);

    // send output
    bool sendBlossomOuput(const std::string &address,
                          const std::string &output);





    HostHandler* m_hostHandler = nullptr;
    Kitsunemimi::Sakura::SessionController* m_sessionController = nullptr;

    std::string m_address = "127.0.0.1";
    std::string m_hostName = "";

    void* m_target = nullptr;
    void (*m_incomingHost)(void*,
                           const std::string);
    void (*m_incomingData)(void*,
                           const uint8_t,
                           const std::string,
                           const void*,
                           const uint64_t);
    void (*m_incomingSeedTrigger)(void*,
                                  const std::string,
                                  const std::string,
                                  Session*,
                                  const uint64_t);
    void (*m_incomingBlossomOutput)(void*,
                                    const std::string,
                                    const std::string);
};

#endif // SAKURA_HOST_HANDLER_H
