/**
 * @file       host_handler.cpp
 *
 * @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright  Apache License Version 2.0
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

#include "host_handler.h"
#include <libKitsunemimiProjectNetwork/session.h>

/**
 * @brief HostHandler::HostHandler
 */
HostHandler::HostHandler() {}

/**
 * @brief HostHandler::addHost
 * @param hostName
 * @param tag
 * @param session
 * @return
 */
bool
HostHandler::addHost(const std::string &hostName,
                     const std::vector<std::string> &tags,
                     Kitsunemimi::Project::Session* session)
{
    std::map<std::string, Host>::const_iterator it;
    it = m_hosts.find(hostName);

    if(it != m_hosts.end()) {
        return false;
    }

    Host newHost;
    newHost.tags = tags;
    newHost.session = session;

    m_hosts.insert(std::make_pair(hostName, newHost));

    return true;
}

/**
 * @brief HostHandler::updateHost
 * @param hostName
 * @param session
 * @return
 */
bool
HostHandler::updateHost(const std::string &hostName,
                        Kitsunemimi::Project::Session* session)
{
    std::map<std::string, Host>::iterator it;
    it = m_hosts.find(hostName);

    if(it == m_hosts.end()) {
        return false;
    }

    if(it->second.session != nullptr) {
        return false;
    }

    it->second.session = session;

    return true;
}

/**
 * @brief HostHandler::get
 * @param hostName
 * @return
 */
Kitsunemimi::Project::Session*
HostHandler::get(const std::string &hostName)
{
    std::map<std::string, Host>::const_iterator it;
    it = m_hosts.find(hostName);

    if(it != m_hosts.end()) {
        return it->second.session;
    }

    return nullptr;
}

/**
 * @brief HostHandler::getByTag
 * @param tag
 * @return
 */
const std::vector<Kitsunemimi::Project::Session*>
HostHandler::getByTag(const std::string &tag)
{
    std::vector<Kitsunemimi::Project::Session*> result;

    std::map<std::string, Host>::const_iterator it;
    for(it = m_hosts.begin();
        it != m_hosts.end();
        it++)
    {
        for(uint32_t i = 0; i < it->second.tags.size(); i++)
        {
            if(it->second.tags.at(i) == tag) {
                result.push_back(it->second.session);
            }
        }
    }

    return result;
}

/**
 * @brief HostHandler::getAll
 * @return
 */
const std::vector<Kitsunemimi::Project::Session*>
HostHandler::getAll()
{
    std::vector<Kitsunemimi::Project::Session*>  result;

    std::map<std::string, Host>::const_iterator it;
    for(it = m_hosts.begin();
        it != m_hosts.end();
        it++)
    {
        result.push_back(it->second.session);
    }

    return result;
}

/**
 * @brief HostHandler::remove
 * @param hostName
 * @return
 */
bool
HostHandler::remove(const std::string &hostName)
{
    std::map<std::string, Host>::iterator it;
    it = m_hosts.find(hostName);

    if(it != m_hosts.end())
    {
        // TODO: close session
        m_hosts.erase(it);
        return true;
    }

    return false;
}
