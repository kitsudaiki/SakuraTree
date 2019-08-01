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
#include <threading/commonThread.hpp>

namespace SakuraSeed
{

class SakuraThread : public Kitsune::CommonThread
{

public:
    SakuraThread(JsonObject *growPlan,
                 JsonObject *values,
                 const std::vector<std::string> &hirarchie);
    ~SakuraThread();

    void waitUntilStarted();
    bool isAborted() const;

private:
    bool m_started = false;

    std::vector<SakuraThread*> m_childs;

    JsonObject* m_growPlan;
    JsonObject* m_values;
    std::vector<std::string> m_hirarchie;

    void run();

    void grow(JsonObject *growPlan,
              JsonObject *values,
              const std::vector<std::string> &hirarchie);

    void processBlossom(JsonObject* growPlan,
                        JsonObject* values,
                        const std::vector<std::string> &hirarchie);

    void processBranch(JsonObject* growPlan,
                       JsonObject* values,
                       const std::vector<std::string> &hirarchie);

    void processForest(JsonObject* growPlan,
                       JsonObject* values,
                       const std::vector<std::string> &hirarchie);

    void processArea(JsonObject* growPlan,
                     JsonObject* values,
                     const std::vector<std::string> &hirarchie);

    void processLandscape(JsonObject* growPlan,
                          JsonObject* values,
                          const std::vector<std::string> &hirarchie);

    void processSequeniellPart(JsonObject* growPlan,
                               JsonObject* values,
                               const std::vector<std::string> &hirarchie);

    void processParallelPart(JsonObject* growPlan,
                             JsonObject* values,
                             const std::vector<std::string> &hirarchie);

    void clearChilds();
};

}

#endif // SAKURATHREAD_H
