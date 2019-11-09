/**
 * @file        sakura_compiler.h
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

#ifndef SAKURA_COMPILER_H
#define SAKURA_COMPILER_H

#include <common.h>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>

namespace Kitsunemimi
{
namespace Sakura
{
class SakuraConverter;
}
}

namespace SakuraTree
{
class FileCollector;
class SakuraItem;

class SakuraCompiler
{
public:
    SakuraCompiler(Kitsunemimi::Sakura::SakuraConverter* driver);
    ~SakuraCompiler();

    SakuraItem* compile(const std::string &rootPath,
                        std::string &seedName);

private:
    Kitsunemimi::Sakura::SakuraConverter* m_driver = nullptr;
    FileCollector* m_fileCollector = nullptr;

    void preProcessObject(DataMap* object);
    void preProcessArray(DataArray* object);

    SakuraItem* convert(DataMap* growPlan);
    SakuraItem* convertBlossom(DataMap* growPlan);
    SakuraItem* convertBranch(DataMap* growPlan);
    SakuraItem* convertTree(DataMap* growPlan);
    SakuraItem* convertSeed(DataMap* growPlan);

    SakuraItem* convertSequeniellPart(DataMap* growPlan);
    SakuraItem* convertParallelPart(DataMap* growPlan);
};

}

#endif // SAKURA_COMPILER_H
