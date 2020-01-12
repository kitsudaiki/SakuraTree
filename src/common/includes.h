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
#include <istream>
#include <iterator>
#include <queue>
#include <sys/ioctl.h>
#include <unistd.h>

#include <boost/filesystem.hpp>

#include <libKitsunemimiCommon/common_items/data_items.h>
#include <libKitsunemimiJson/json_item.h>

#include <libKitsunemimiCommon/process_execution.h>

using Kitsunemimi::Common::DataItem;
using Kitsunemimi::Common::DataArray;
using Kitsunemimi::Common::DataValue;
using Kitsunemimi::Common::DataMap;

using Kitsunemimi::Json::JsonItem;

using Kitsunemimi::Common::ProcressResult;
using Kitsunemimi::Common::runSyncProcess;

#endif // INCLUDES_H
