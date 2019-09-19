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

namespace SakuraTree
{
struct BlossomItem;

std::string convertString(const std::string &templateString,
                          DataMap* content);
DataMap* fillItems(DataMap* items,
                   DataMap* insertValues);
DataMap* overrideItems(DataMap* original,
                          DataMap* override);

std::vector<std::string> checkItems(DataMap* items);

void printOutput(BlossomItem* blossom);

}

#endif // COMMON_METHODS_H
