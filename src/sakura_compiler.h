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

namespace Kitsunemimi
{
namespace Sakura
{
class SakuraParsing;
}
}

namespace SakuraTree
{
class FileCollector;
class SakuraItem;
class BlossomItem;

class SakuraCompiler
{
public:
    SakuraCompiler();
    ~SakuraCompiler();

    SakuraItem* compile(JsonItem &tree);

private:
    SakuraItem* convert(JsonItem &growPlan,
                        SakuraItem* parent);

    BlossomItem* convertBlossom(JsonItem &growPlan,
                                SakuraItem* parent);
    SakuraItem* convertBlossomGroup(JsonItem &growPlan,
                                    SakuraItem* parent);
    SakuraItem* convertBranch(JsonItem &growPlan,
                              SakuraItem* parent);
    SakuraItem* convertTree(JsonItem &growPlan,
                            SakuraItem* parent);
    SakuraItem* convertSeed(JsonItem &growPlan,
                            SakuraItem* parent);
    SakuraItem* convertIf(JsonItem &growPlan,
                          SakuraItem* parent);

    SakuraItem* convertSequeniellPart(JsonItem &growPlan,
                                      SakuraItem* parent);
    SakuraItem* convertParallelPart(JsonItem &growPlan,
                                    SakuraItem* parent);
};

}

#endif // SAKURA_COMPILER_H
