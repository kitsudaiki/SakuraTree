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
#include <items/value_items.h>

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

    SakuraItem* compile(const JsonItem &tree);

private:
    SakuraItem* convert(const JsonItem &growPlan,
                        SakuraItem* parent);

    void overrideItems(JsonItem &original,
                       const JsonItem &override);
    bool convertItemPart(ValueItem &resultingPart,
                         JsonItem itemInput,
                         const std::string pairType);
    void convertValues(SakuraItem* obj,
                       const JsonItem &values);

    SakuraItem* convertBlossomGroup(const JsonItem &growPlan,
                                    SakuraItem* parent);
    SakuraItem* convertBranch(const JsonItem &growPlan,
                              SakuraItem* parent);
    SakuraItem* convertTree(const JsonItem &growPlan,
                            SakuraItem* parent);
    SakuraItem* convertSeed(const JsonItem &growPlan,
                            SakuraItem* parent);
    SakuraItem* convertIf(const JsonItem &growPlan,
                          SakuraItem* parent);
    SakuraItem* convertForEach(const JsonItem &growPlan,
                               SakuraItem* parent,
                               bool parallel);
    SakuraItem* convertFor(const JsonItem &growPlan,
                           SakuraItem* parent,
                           bool parallel);
    SakuraItem* convertParallel(const JsonItem &subtree,
                                SakuraItem* parent);

    SakuraItem* convertSequeniellPart(const JsonItem &growPlan,
                                      SakuraItem* parent);
    SakuraItem* convertParallelPart(const JsonItem &growPlan,
                                    SakuraItem* parent);
};

}

#endif // SAKURA_COMPILER_H
