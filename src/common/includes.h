/**
 * @file        includes.h
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#ifndef INCLUDES_H
#define INCLUDES_H

#include <assert.h>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <streambuf>
#include <thread>
#include <istream>
#include <iterator>
#include <queue>
#include <sys/ioctl.h>
#include <unistd.h>

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
using Kitsunemimi::Sakura::SakuraLangInterface;

#include <libKitsunemimiSakuraLang/blossom.h>
using Kitsunemimi::Sakura::Blossom;
using Kitsunemimi::Sakura::BlossomLeaf;

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

#include <libKitsunemimiCommon/common_items/data_items.h>
using Kitsunemimi::DataItem;
using Kitsunemimi::DataArray;
using Kitsunemimi::DataValue;
using Kitsunemimi::DataMap;

#include <libKitsunemimiPersistence/logger/logger.h>

#include <libKitsunemimiJson/json_item.h>
using Kitsunemimi::Json::JsonItem;

#include <libKitsunemimiCommon/process_execution.h>
using Kitsunemimi::ProcessResult;
using Kitsunemimi::runSyncProcess;

#endif // INCLUDES_H
