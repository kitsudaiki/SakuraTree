/**
 * @file        value_item_functions.h
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

#ifndef VALUE_ITEM_FUNCTIONS_H
#define VALUE_ITEM_FUNCTIONS_H

#include <common.h>

DataItem* getValue(DataItem* item, DataValue* key, std::string &errorMessage);
DataArray* splitValue(DataValue* item, DataValue* delimiter, std::string &errorMessage);
DataValue* sizeValue(DataItem* item, std::string &errorMessage);
DataValue* containsValue(DataItem* item, DataValue* key, std::string &errorMessage);
DataArray* appendValue(DataArray* item, DataItem* value, std::string &errorMessage);
DataMap* insertValue(DataMap* item, DataValue* key, DataItem* value, std::string &errorMessage);
DataArray* clearEmpty(DataArray* item, std::string &errorMessage);
DataItem* parseJson(DataValue* intput, std::string &errorMessage);


#endif // VALUE_ITEM_FUNCTIONS_H
