/**
 *  @file    commonMethods.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef COMMONMETHODS_H
#define COMMONMETHODS_H

#include <common.h>

namespace SakuraTree
{
struct BlossomData;

std::string convertString(const std::string &templateString,
                          JsonObject* content);
JsonObject* fillItems(JsonObject* items,
                      JsonObject* insertValues);
JsonObject* overrideItems(JsonObject* original,
                          JsonObject* override);

std::vector<std::string> checkItems(JsonObject* items);

void printOutput(BlossomData* blossom);

}

#endif // COMMONMETHODS_H
