/**
 *  @file    sakuraThread.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURATHREAD_H
#define SAKURATHREAD_H

#include <common.h>
#include <threading/thread.hpp>

using namespace Kitsune::Common;

namespace SakuraTree
{

class SakuraThread
        : public Kitsune::Common::Thread
{

public:
    SakuraThread(DataObject *growPlan,
                 DataObject *values,
                 const std::vector<std::string> &hirarchie);
    ~SakuraThread();

    void waitUntilStarted();
    bool isAborted() const;

private:
    bool m_started = false;

    std::vector<SakuraThread*> m_childs;

    DataObject* m_growPlan;
    DataObject* m_values;
    std::vector<std::string> m_hirarchie;

    void run();

    void grow(DataObject *growPlan,
              DataObject *values,
              const std::vector<std::string> &hirarchie);

    void processBlossom(DataObject* growPlan,
                        DataObject* values,
                        const std::vector<std::string> &hirarchie);

    void processBranch(DataObject* growPlan,
                       DataObject* values,
                       const std::vector<std::string> &hirarchie);

    void processForest(DataObject* growPlan,
                       DataObject* values,
                       const std::vector<std::string> &hirarchie);

    void processArea(DataObject* growPlan,
                     DataObject* values,
                     const std::vector<std::string> &hirarchie);

    void processLandscape(DataObject* growPlan,
                          DataObject* values,
                          const std::vector<std::string> &hirarchie);

    void processSequeniellPart(DataObject* growPlan,
                               DataObject* values,
                               const std::vector<std::string> &hirarchie);

    void processParallelPart(DataObject* growPlan,
                             DataObject* values,
                             const std::vector<std::string> &hirarchie);

    void clearChilds();
};

}

#endif // SAKURATHREAD_H
