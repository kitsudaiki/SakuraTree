/**
 * @file        converter.h
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

#ifndef CONVERTER_H
#define CONVERTER_H

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
class SakuraItem;
class BlossomItem;

class Converter
{
public:
    Converter();
    ~Converter();

    SakuraItem* convert(const JsonItem &tree,
                        const bool isSeed=false);

private:
    bool m_isSeed = true;

    SakuraItem* convertPart(const JsonItem &subtree, bool &success);

    void convertSingleItemValue(ValueItem &resultingPart,
                                const JsonItem &itemInput,
                                const std::string pairType,
                                bool &success);
    void convertItemValues(SakuraItem* sakuraItem,
                           const JsonItem &jsonValues,
                           bool &success);

    SakuraItem* convertBlossomGroup(const JsonItem &subtree, bool &success);
    SakuraItem* convertTree(const JsonItem &subtree, bool &success);
    SakuraItem* convertSubtree(const JsonItem &subtree, bool &success);
    SakuraItem* convertSeed(const JsonItem &subtree, bool &success);
    SakuraItem* convertIf(const JsonItem &subtree, bool &success);
    SakuraItem* convertForEach(const JsonItem &subtree, const bool parallel, bool &success);
    SakuraItem* convertFor(const JsonItem &subtree, const bool parallel, bool &success);
    SakuraItem* convertParallel(const JsonItem &subtree, bool &success);
};

}

#endif // CONVERTER_H
