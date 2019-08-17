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
                          DataObject* content);
DataObject* fillItems(DataObject* items,
                      DataObject* insertValues);
DataObject* overrideItems(DataObject* original,
                          DataObject* override);

std::vector<std::string> checkItems(DataObject* items);

void printOutput(BlossomData* blossom);

}

#endif // COMMON_METHODS_H
