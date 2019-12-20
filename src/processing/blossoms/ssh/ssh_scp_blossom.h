/**
 * @file        ssh_scp_blossom.h
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

#ifndef SCP_BLOSSOM_H
#define SCP_BLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraTree
{

class SshScpBlossom
        : public Blossom
{
public:
    SshScpBlossom();

protected:
    void initTask(BlossomItem &blossomItem);
    void preCheck(BlossomItem &blossomItem);
    void runTask(BlossomItem &blossomItem);
    void postCheck(BlossomItem &blossomItem);
    void closeTask(BlossomItem &blossomItem);

private:
    std::string m_user = "";
    std::string m_address = "";
    std::string m_port = "";
    std::string m_sshKey = "";
    std::string m_sourcePath = "";
    std::string m_targetPath = "";
};

}

#endif // SCP_BLOSSOM_H
