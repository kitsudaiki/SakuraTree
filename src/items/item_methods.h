/**
 *  @file    common_methods.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef COMMON_METHODS_H
#define COMMON_METHODS_H

#include <common.h>

using Kitsunemimi::Common::DataMap;
namespace SakuraTree
{
struct BlossomItem;

const std::string convertString(const std::string &templateString,
                                DataMap* content);
void fillItems(DataMap &items,
               Kitsunemimi::Common::DataMap &insertValues);

void overrideItems(DataMap &original,
                   DataMap &override);

const std::vector<std::string> checkItems(DataMap &items);

void printOutput(const BlossomItem &blossom);

}

#endif // COMMON_METHODS_H
