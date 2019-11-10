/**
 * @file        provision_branch_builder.cpp
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

#include "provision_branch_builder.h"
#include <items/sakura_items.h>

namespace SakuraTree
{

/**
 * @brief createProvisionBranch
 * @param address
 * @param port
 * @param userName
 * @param keyPath
 * @param sakaraTreePath
 * @param targetPath
 * @param subtree
 * @return
 */
BranchItem*
createProvisionBranch(const std::string &address,
                      const int port,
                      const std::string &userName,
                      const std::string &keyPath,
                      const std::string &sakaraTreePath,
                      const std::string &targetPath,
                      const std::string &subtree)
{
    BranchItem* item = new BranchItem();

    BlossomItem* updateBlossom = createAptUpdateBlossom(address,
                                                        port,
                                                        userName,
                                                        keyPath);
    item->childs.push_back(updateBlossom);
    BlossomItem* installBlossom = createAptLatesBlossom(address,
                                                        port,
                                                        userName,
                                                        keyPath);
    item->childs.push_back(installBlossom);

    BlossomItem* scpBlossom = createScpBlossom(address,
                                               port,
                                               userName,
                                               keyPath,
                                               sakaraTreePath,
                                               targetPath);
    item->childs.push_back(scpBlossom);

    BlossomItem* sshBlossom = createSshBlossom(address,
                                               port,
                                               userName,
                                               keyPath);
    item->childs.push_back(sshBlossom);

    BlossomItem* copySubtreeBlossom = createCopySubtreeBlossom(address, subtree);
    item->childs.push_back(copySubtreeBlossom);

    return item;
}

/**
 * @brief createAptUpdateBlossom
 * @param address
 * @param port
 * @param userName
 * @param keyPath
 * @return
 */
BlossomItem*
createAptUpdateBlossom(const std::string &address,
                       const int port,
                       const std::string &userName,
                       const std::string &keyPath)
{
    BlossomItem* item = new BlossomItem();

    item->blossomType = "ssh";
    item->blossomSubTypes.push_back("cmd");

    item->values.insert("address", new DataValue(address));
    item->values.insert("user", new DataValue(userName));
    item->values.insert("port", new DataValue(port));
    item->values.insert("ssh_key", new DataValue(keyPath));
    item->values.insert("command", new DataValue("sudo apt-get update"));

    return item;
}

/**
 * @brief createAptLatesBlossom
 * @param address
 * @param port
 * @param userName
 * @param keyPath
 * @return
 */
BlossomItem*
createAptLatesBlossom(const std::string &address,
                      const int port,
                      const std::string &userName,
                      const std::string &keyPath)
{
    BlossomItem* item = new BlossomItem();

    item->blossomType = "ssh";
    item->blossomSubTypes.push_back("cmd");

    item->values.insert("address", new DataValue(address));
    item->values.insert("user", new DataValue(userName));
    item->values.insert("port", new DataValue(port));
    item->values.insert("ssh_key", new DataValue(keyPath));
    item->values.insert("command", new DataValue("sudo apt-get install -y libboost-filesystem-dev libsqlite3-dev libboost-program-options-dev"));

    return item;
}


/**
 * @brief createScpBlossom
 * @param address
 * @param port
 * @param userName
 * @param keyPath
 * @param sakaraTreePath
 * @param targetPath
 * @return
 */
BlossomItem*
createScpBlossom(const std::string &address,
                 const int port,
                 const std::string &userName,
                 const std::string &keyPath,
                 const std::string &sakaraTreePath,
                 const std::string &targetPath)
{
    BlossomItem* item = new BlossomItem();

    item->blossomType = "ssh";
    item->blossomSubTypes.push_back("scp");

    item->values.insert("address", new DataValue(address));
    item->values.insert("user", new DataValue(userName));
    item->values.insert("port", new DataValue(port));
    item->values.insert("ssh_key", new DataValue(keyPath));
    item->values.insert("source_path", new DataValue(sakaraTreePath));
    item->values.insert("target_path", new DataValue(targetPath));

    return item;
}

/**
 * @brief createSshBlossom
 * @param address
 * @param port
 * @param userName
 * @param keyPath
 * @return
 */
BlossomItem*
createSshBlossom(const std::string &address,
                 const int port,
                 const std::string &userName,
                 const std::string &keyPath)
{
    BlossomItem* item = new BlossomItem();

    item->blossomType = "ssh";
    item->blossomSubTypes.push_back("cmd");

    item->values.insert("address", new DataValue(address));
    item->values.insert("user", new DataValue(userName));
    item->values.insert("port", new DataValue(port));
    item->values.insert("ssh_key", new DataValue(keyPath));
    item->values.insert("command", new DataValue("~/SakuraTree --server-address 127.0.0.1 --server-port 1337 &"));

    return item;
}

/**
 * @brief createCopySubtreeBlossom
 * @param address
 * @param subtree
 * @return
 */
BlossomItem*
createCopySubtreeBlossom(const std::string &address,
                         const std::string &subtree)
{
    BlossomItem* item = new BlossomItem();

    item->blossomType = "sakura-connection";
    item->blossomSubTypes.push_back("copy-subtree");

    item->values.insert("address", new DataValue(address));
    item->values.insert("subtree", new DataValue(subtree));

    return item;
}

}
