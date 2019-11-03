/**
 *  @file    apt_methods.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef APT_METHODS_H
#define APT_METHODS_H

#include <common.h>
#include <items/sakura_items.h>

namespace SakuraTree
{

bool isInstalled(BlossomItem &blossomItem,
                 const std::string &paket);
void fillPackageNames(BlossomItem &blossomItem,
                      std::vector<std::string> &packageList);
const std::string createPackageList(const std::vector<std::string> &packageList);

const std::vector<std::string> getInstalledPackages(BlossomItem &blossomItem,
                                                    const std::vector<std::string> &packageList);
const std::vector<std::string> getAbsendPackages(BlossomItem &blossomDataconst,
                                                 const std::vector<std::string> &packageList);
const std::vector<std::string> getInstalledPackages(BlossomItem &blossomItem);

}

#endif // APT_METHODS_H
