/**
 *  @file    aptBlossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APTBLOSSOM_H
#define APTBLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraTree
{

class AptBlossom : public Blossom
{
public:
    AptBlossom();

protected:
    std::vector<std::string> m_packageNames;

    virtual void initTask(BlossomData *blossomData) = 0;
    virtual void preCheck(BlossomData *blossomData) = 0;
    virtual void runTask(BlossomData *blossomData) = 0;
    virtual void postCheck(BlossomData *blossomData) = 0;
    virtual void closeTask(BlossomData *blossomData) = 0;

    bool isInstalled(BlossomData *blossomData, const std::string &paket);
    void fillPackageNames(BlossomData* blossomData);
    std::string createPackageList();

    std::vector<std::string> getInstalledPackages(BlossomData *blossomData,
                                                  const std::vector<std::string> &packageList);
    std::vector<std::string> getAbsendPackages(BlossomData* blossomDataconst,
                                               const std::vector<std::string> &packageList);
    std::vector<std::string> getInstalledPackages(BlossomData *blossomData);
};

}

#endif // APTBLOSSOM_H
