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

namespace SakuraTree
{

void dataCallback(void* target,
                  const std::string address,
                  const std::string plan,
                  const std::string values)
{
    std::cout<<"dataCallback: "<<plan<<std::endl;
    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
    rootClass->startSubtreeProcess(plan, values);
}

void blossomOutputCallback(void* target,
                           const std::string address,
                           const std::string output)
{
    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
    rootClass->printOutput(output);
}


void sessionCallback(void* target,
                     const std::string)
{
    SakuraRoot* rootClass = static_cast<SakuraRoot*>(target);
}

}

#endif // SAKURA_TREE_CALLBACKS_H
