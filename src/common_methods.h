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
struct BlossomData;

std::string convertString(const std::string &templateString,
                          DataMap* content);
DataMap* fillItems(DataMap* items,
                      DataMap* insertValues);
DataMap* overrideItems(DataMap* original,
                          DataMap* override);

std::vector<std::string> checkItems(DataMap* items);

void printOutput(BlossomData* blossom);

}

#endif // COMMON_METHODS_H
