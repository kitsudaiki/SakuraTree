/**
 * @file       host_handler.h
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

#ifndef HOST_MAP_H
#define HOST_MAP_H

#include <iostream>
#include <map>
#include <vector>
#include <utility>

namespace Kitsunemimi
{
namespace Project {
class Session;
}
}

class HostHandler
{

struct Host
{
    std::vector<std::string> tags;
    Kitsunemimi::Project::Session* session = nullptr;
};

public:
    HostHandler();

    bool addHost(const std::string &hostName,
                 const std::vector<std::string> &tags,
                 Kitsunemimi::Project::Session* session=nullptr);
    bool updateHost(const std::string &hostName,
                    Kitsunemimi::Project::Session* session);

    Kitsunemimi::Project::Session* get(const std::string &hostName);
    const std::vector<Kitsunemimi::Project::Session*> getByTag(const std::string &tag);
    const std::vector<Kitsunemimi::Project::Session*> getAll();

    bool remove(const std::string &hostName);

private:
    std::map<std::string, Host> m_hosts;
};

#endif // HOST_MAP_H
