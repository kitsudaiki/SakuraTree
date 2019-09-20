/**
 *  @file    sakura_thread.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURA_THREAD_H
#define SAKURA_THREAD_H

#include <common.h>
#include <threading/thread.h>
#include <items/sakura_items.h>

using namespace Kitsune::Common;

namespace SakuraTree
{

class SakuraThread
        : public Kitsune::Common::Thread
{

public:
    SakuraThread(SakuraItem* growPlan,
                 DataMap *values,
                 const std::vector<std::string> &hirarchie);
    ~SakuraThread();

    void waitUntilStarted();
    bool isAborted() const;

private:
    bool m_started = false;

    std::vector<SakuraThread*> m_childs;

    SakuraItem* m_growPlan;
    DataMap* m_values;
    std::vector<std::string> m_hirarchie;

    void run();

    void grow(SakuraItem* growPlan,
              DataMap* values,
              const std::vector<std::string> &hirarchie);

    void processBlossom(BlossomItem &growPlan,
                        DataMap* values,
                        const std::vector<std::string> &hirarchie);

    void processBranch(BranchItem* growPlan,
                       DataMap* values,
                       const std::vector<std::string> &hirarchie);

    void processTree(TreeItem* growPlan,
                     DataMap* values,
                     const std::vector<std::string> &hirarchie);

    void processSequeniellPart(SequeniellBranching* growPlan,
                               DataMap* values,
                               const std::vector<std::string> &hirarchie);

    void processParallelPart(ParallelBranching* growPlan,
                             DataMap* values,
                             const std::vector<std::string> &hirarchie);

    void clearChilds();
};

}

#endif // SAKURA_THREAD_H
