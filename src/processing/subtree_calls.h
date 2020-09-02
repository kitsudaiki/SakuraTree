#ifndef SUBTREE_CALLS_H
#define SUBTREE_CALLS_H

#include <common.h>

namespace Kitsunemimi
{
namespace Project
{
class Session;
}
}

bool spawnParallelSubtrees(const std::vector<SakuraItem *> &childs,
                           const uint64_t startPos,
                           const uint64_t endPos,
                           const std::string &filePath,
                           const std::vector<std::string> &hierarchy,
                           const DataMap &parentValues,
                           std::string &errorMessage,
                           Kitsunemimi::Project::Session* session = nullptr,
                           const uint64_t blockerId = 0);
bool spawnParallelSubtreesLoop(SakuraItem* subtree,
                               const uint64_t startPos,
                               const uint64_t endPos,
                               const std::string &filePath,
                               const std::vector<std::string> &hierarchy,
                               DataMap &parentValues,
                               const std::string &tempVarName,
                               DataArray* array,
                               std::string &errorMessage);

#endif // SUBTREE_CALLS_H
