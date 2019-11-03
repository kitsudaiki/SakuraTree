/**
 *  @file    sakura_compiler.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
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

    void preProcessObject(DataMap* value);
    void preProcessArray(DataArray* value);

    SakuraItem* convert(DataMap* growPlan);
    SakuraItem* convertBlossom(DataMap* growPlan);
    SakuraItem* convertBranch(DataMap* growPlan);
    SakuraItem* convertTree(DataMap* growPlan);
    SakuraItem* convertForest(DataMap* growPlan);

    SakuraItem* convertSequeniellPart(DataMap* growPlan);
    SakuraItem* convertParallelPart(DataMap* growPlan);
};

}

#endif // SAKURA_COMPILER_H
