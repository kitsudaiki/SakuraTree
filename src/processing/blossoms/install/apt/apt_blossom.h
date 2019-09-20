/**
 *  @file    apt_blossom.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APT_BLOSSOM_H
#define APT_BLOSSOM_H

#include <processing/blossoms/blossom.h>

namespace SakuraTree
{

class AptBlossom : public Blossom
{
public:
    AptBlossom();

protected:
    std::vector<std::string> m_packageNames;

    virtual void initTask(BlossomItem &blossomItem) = 0;
    virtual void preCheck(BlossomItem &blossomItem) = 0;
    virtual void runTask(BlossomItem &blossomItem) = 0;
    virtual void postCheck(BlossomItem &blossomItem) = 0;
    virtual void closeTask(BlossomItem &blossomItem) = 0;

    bool isInstalled(BlossomItem &blossomItem, const std::string &paket);
    void fillPackageNames(BlossomItem &blossomItem);
    std::string createPackageList();

    std::vector<std::string> getInstalledPackages(BlossomItem &blossomItem,
                                                  const std::vector<std::string> &packageList);
    std::vector<std::string> getAbsendPackages(BlossomItem &blossomDataconst,
                                               const std::vector<std::string> &packageList);
    std::vector<std::string> getInstalledPackages(BlossomItem &blossomItem);
};

}

#endif // APT_BLOSSOM_H
