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

BranchItem*
createProvisionBranch(const std::string address,
                      const int port,
                      const std::string userName,
                      const std::string keyPath,
                      const std::string sakaraTreePath,
                      const std::string targetPath)
{
    BranchItem* item = new BranchItem();

    BlossomItem* scpBlossom = createScpBlossom(address,
                                               port,
                                               userName,
                                               keyPath,
                                               sakaraTreePath,
                                               targetPath);
    item->childs.push_back(scpBlossom);

    return item;
}

BlossomItem*
createScpBlossom(const std::string address,
                 const int port,
                 const std::string userName,
                 const std::string keyPath,
                 const std::string sakaraTreePath,
                 const std::string targetPath)
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

}