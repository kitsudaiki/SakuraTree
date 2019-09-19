/**
 *  @file    sakura_thread.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "sakura_thread.h"

#include <sakura_root.h>

#include <items/item_methods.h>

#include <processing/blossoms/blossom.h>
#include <processing/blossoms/blossom_getter.h>

namespace SakuraTree
{

/**
 * constructor
 */
SakuraThread::SakuraThread(SakuraItem* growPlan,
                           DataMap* values,
                           const std::vector<std::string> &hirarchie)
{
    assert(growPlan != nullptr);
    assert(values != nullptr);

    m_values = values;
    m_growPlan = growPlan;
    m_hirarchie = hirarchie;
}

/**
 * destructor
 */
SakuraThread::~SakuraThread()
{
    clearChilds();
}

/**
 * wait until the thread was started
 */
void
SakuraThread::waitUntilStarted()
{
    while(m_started == false)
    {
        std::this_thread::sleep_for(chronoMicroSec(10));
    }
}

/**
 * check if the thread was aborted
 *
 * @return true, if thread was aborted, else false
 */
bool
SakuraThread::isAborted() const
{
    return m_abort;
}

/**
 * start thread
 */
void
SakuraThread::run()
{
    m_started = true;
    grow(m_growPlan, m_values, m_hirarchie);
}

/**
 * central method of the thread to process the current part of the execution-tree
 */
void
SakuraThread::grow(SakuraItem* growPlan,
                   DataMap* values,
                   const std::vector<std::string> &hirarchie)
{
    if(m_abort) {
        return;
    }

    if(growPlan->getType() == SakuraItem::BLOSSOM_ITEM)
    {
        BlossomItem* blossomItem = dynamic_cast<BlossomItem*>(growPlan);
        fillItems(&blossomItem->values, values);
        std::vector<std::string> newHirarchie = hirarchie;
        newHirarchie.push_back("BLOSSOM: " + blossomItem->name);
        processBlossom(blossomItem, &blossomItem->values, newHirarchie);
        return;
    }

    if(growPlan->getType() == SakuraItem::BRANCH_ITEM)
    {
        BranchItem* branchItem = dynamic_cast<BranchItem*>(growPlan);
        fillItems(&branchItem->values, values);
        processBranch(branchItem, &branchItem->values, hirarchie);
        return;
    }

    if(growPlan->getType() == SakuraItem::TREE_ITEM)
    {
        TreeItem* treeItem = dynamic_cast<TreeItem*>(growPlan);
        fillItems(&treeItem->values, values);
        processTree(treeItem, &treeItem->values, hirarchie);
        return;
    }

    if(growPlan->getType() == SakuraItem::SEQUENTIELL_ITEM)
    {
        processSequeniellPart(dynamic_cast<SequeniellBranching*>(growPlan), values, hirarchie);
        return;
    }

    if(growPlan->getType() == SakuraItem::PARALLEL_ITEM)
    {
        processParallelPart(dynamic_cast<ParallelBranching*>(growPlan), values, hirarchie);
        return;
    }

    return;
}

/**
 * @brief SakuraThread::processBlossom
 */
void
SakuraThread::processBlossom(BlossomItem* growPlan,
                             DataMap* values,
                             const std::vector<std::string> &hirarchie)
{
    // init
    growPlan->values = *dynamic_cast<DataMap*>(values->copy());
    growPlan->nameHirarchie = hirarchie;

    // iterate over all subtypes and execute each as separate blossom
    for(uint32_t i = 0; i < growPlan->blossomSubTypes.size(); i++)
    {
        std::string subtype = growPlan->blossomSubTypes.at(i);
        Blossom* blossom = getBlossom(growPlan->blossomType, subtype);
        blossom->growBlossom(growPlan);
        delete blossom;
    }

    // abort if blossom-result was an error
    if(growPlan->success == false)
    {
        m_abort = true;
        std::string output = "ABORT after ERROR";
        SakuraRoot::m_root->addMessage(growPlan);
    }

    // send result to root
    SakuraRoot::m_root->addMessage(growPlan);

    return;
}

/**
 * @brief SakuraThread::processBranch
 */
void
SakuraThread::processBranch(BranchItem* growPlan,
                            DataMap* values,
                            const std::vector<std::string> &hirarchie)
{
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             values,
             hirarchie);
    }

    return;
}

/**
 * @brief SakuraThread::processTree
 */
void
SakuraThread::processTree(TreeItem* growPlan,
                          DataMap* values,
                          const std::vector<std::string> &hirarchie)
{
    // create and initialize all threads
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        SakuraThread* child = new SakuraThread(growPlan->childs.at(i),
                                               values,
                                               hirarchie);
        m_childs.push_back(child);
        child->start();
    }

    // wait for the end of all threads
    for(uint32_t i = 0; i < m_childs.size(); i++)
    {
        // wait the process is startet, so the join-method can not be called
        // before the new thread was really started
        m_childs.at(i)->waitUntilStarted();

        // finish the threads after they done their tasks
        m_childs.at(i)->waitForFinish();

        // check if any child was aborted
        if(m_childs.at(i)->isAborted()) {
            m_abort = true;
        }
    }
    clearChilds();

    return;
}

/**
 * @brief SakuraThread::processSequeniellPart
 */
void
SakuraThread::processSequeniellPart(SequeniellBranching* growPlan,
                                    DataMap* values,
                                    const std::vector<std::string> &hirarchie)
{
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        grow(growPlan->childs.at(i),
             values,
             hirarchie);
    }

    return;
}

/**
 * @brief SakuraThread::processParallelPart
 */
void
SakuraThread::processParallelPart(ParallelBranching* growPlan,
                                  DataMap* values,
                                  const std::vector<std::string> &hirarchie)
{
    // create and initialize all threads
    for(uint32_t i = 0; i < growPlan->childs.size(); i++)
    {
        SakuraThread* child = new SakuraThread(growPlan->childs.at(i),
                                               values,
                                               hirarchie);
        m_childs.push_back(child);
        child->start();
    }

    // wait for the end of all threads
    for(uint32_t i = 0; i < m_childs.size(); i++)
    {
        // wait the process is startet, so the join-method can not be called
        // before the new thread was really started
        m_childs.at(i)->waitUntilStarted();

        // finish the threads after they done their tasks
        m_childs.at(i)->waitForFinish();

        // check if any child was aborted
        if(m_childs.at(i)->isAborted()) {
            m_abort = true;
        }
    }
    clearChilds();

    return;
}

/**
 * stop and delete all child processes
 */
void
SakuraThread::clearChilds()
{
    for(uint32_t i = 0; i < m_childs.size(); i++)
    {
        m_childs.at(i)->stop();
        delete m_childs[i];
    }
    m_childs.clear();
}

}
