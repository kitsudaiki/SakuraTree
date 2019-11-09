/**
 * @file        provision_branch_builder.h
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

#ifndef PROVISION_BRANCH_BUILDER_H
#define PROVISION_BRANCH_BUILDER_H

#include <common.h>

namespace SakuraTree
{
class BranchItem;
class BlossomItem;

BranchItem* createProvisionBranch(const std::string &address,
                                  const int port,
                                  const std::string &userName,
                                  const std::string &keyPath,
                                  const std::string &sakaraTreePath,
                                  const std::string &targetPath,
                                  const std::string &subtree);


BlossomItem* createScpBlossom(const std::string &address,
                              const int port,
                              const std::string &userName,
                              const std::string &keyPath,
                              const std::string &sakaraTreePath,
                              const std::string &targetPath);

BlossomItem* createCopySubtreeBlossom(const std::string &address,
                                      const std::string &subtree);
}

#endif // PROVISION_BRANCH_BUILDER_H
