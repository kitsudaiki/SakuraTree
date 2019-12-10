/**
 * @file        sakura_thread.h
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

#ifndef SAKURA_THREAD_H
#define SAKURA_THREAD_H

#include <common.h>
#include <libKitsunemimiCommon/threading/thread.h>
#include <items/sakura_items.h>

using namespace Kitsunemimi::Common;

namespace SakuraTree
{

class SakuraThread
        : public Kitsunemimi::Common::Thread
{

public:
    SakuraThread(SakuraItem* growPlan,
                 ValueItemMap values,
                 const std::vector<std::string> &hierarchy);
    ~SakuraThread();

    void waitUntilStarted();
    bool isAborted() const;

private:
    bool m_started = false;

    std::vector<SakuraThread*> m_childThreads;

    SakuraItem* m_growPlan;
    ValueItemMap m_values;
    std::vector<std::string> m_hirarchie;

    void run();

    void grow(SakuraItem* growPlan,
              ValueItemMap values,
              const std::vector<std::string> &hierarchy);

    void processBlossom(BlossomItem &growPlan,
                        ValueItemMap values,
                        const std::vector<std::string> &hierarchy);

    void processBlossomGroup(BlossomGroupItem &growPlan,
                             ValueItemMap values,
                             const std::vector<std::string> &hierarchy);

    void processBranch(BranchItem* growPlan,
                       ValueItemMap values,
                       const std::vector<std::string> &hierarchy);

    void processTree(TreeItem* growPlan,
                     ValueItemMap values,
                     const std::vector<std::string> &hierarchy);

    void processIf(IfBranching* growPlan,
                   ValueItemMap values,
                   const std::vector<std::string> &hierarchy);

    void processForEach(ForEachBranching* growPlan,
                        ValueItemMap values,
                        const std::vector<std::string> &hierarchy);

    void processFor(ForBranching* growPlan,
                    ValueItemMap values,
                    const std::vector<std::string> &hierarchy);

    void processParallelForEach(ParallelForEachBranching* growPlan,
                                ValueItemMap values,
                                const std::vector<std::string> &hierarchy);

    void processParallelFor(ParallelForBranching* growPlan,
                            ValueItemMap values,
                            const std::vector<std::string> &hierarchy);

    void processSequeniellPart(Sequentiell* growPlan,
                               ValueItemMap values,
                               const std::vector<std::string> &hierarchy);

    void processParallelPart(Parallel* growPlan,
                             ValueItemMap values,
                             const std::vector<std::string> &hierarchy);

    void clearChildThreads();
};

}

#endif // SAKURA_THREAD_H
