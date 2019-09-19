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

using namespace Kitsune::Common;

namespace SakuraTree
{

class SakuraThread
        : public Kitsune::Common::Thread
{

public:
    SakuraThread(DataMap *growPlan,
                 DataMap *values,
                 const std::vector<std::string> &hirarchie);
    ~SakuraThread();

    void waitUntilStarted();
    bool isAborted() const;

private:
    bool m_started = false;

    std::vector<SakuraThread*> m_childs;

    DataMap* m_growPlan;
    DataMap* m_values;
    std::vector<std::string> m_hirarchie;

    void run();

    void grow(DataMap *growPlan,
              DataMap *values,
              const std::vector<std::string> &hirarchie);

    void processBlossom(DataMap* growPlan,
                        DataMap* values,
                        const std::vector<std::string> &hirarchie);

    void processBranch(DataMap* growPlan,
                       DataMap* values,
                       const std::vector<std::string> &hirarchie);

    void processForest(DataMap* growPlan,
                       DataMap* values,
                       const std::vector<std::string> &hirarchie);

    void processArea(DataMap* growPlan,
                     DataMap* values,
                     const std::vector<std::string> &hirarchie);

    void processLandscape(DataMap* growPlan,
                          DataMap* values,
                          const std::vector<std::string> &hirarchie);

    void processSequeniellPart(DataMap* growPlan,
                               DataMap* values,
                               const std::vector<std::string> &hirarchie);

    void processParallelPart(DataMap* growPlan,
                             DataMap* values,
                             const std::vector<std::string> &hirarchie);

    void clearChilds();
};

}

#endif // SAKURA_THREAD_H
