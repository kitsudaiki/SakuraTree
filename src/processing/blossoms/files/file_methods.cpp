/**
 * @file        file_methods.cpp
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

#include "file_methods.h"

namespace SakuraTree
{

/**
 * @brief doesPathExist
 * @param path
 * @return
 */
std::pair<bool, std::string>
doesPathExist(const std::string path)
{
    std::pair<bool, std::string> result;
    boost::filesystem::path filePathObj(path);

    if(exists(path) == false)
    {
        result.first = false;
        result.second = "path doesn't exist: " + path;
        return result;
    }

    result.first = true;
    result.second = "";
    return result;
}

/**
 * @brief doesFileExist
 * @param filePath
 * @return
 */
std::pair<bool, std::string>
doesFileExist(const std::string filePath)
{
    std::pair<bool, std::string> result;
    boost::filesystem::path filePathObj(filePath);

    if(exists(filePath) == false)
    {
        result.first = false;
        result.second = "path doesn't exist: " + filePath;
        return result;
    }

    if(is_directory(filePath))
    {
        result.first = false;
        result.second = "path exist, but is a directory: " + filePath;
        return result;
    }

    result.first = true;
    result.second = "";
    return result;
}

/**
 * @brief doesDirExist
 * @param dirPath
 * @return
 */
std::pair<bool, std::string>
doesDirExist(const std::string dirPath)
{
    std::pair<bool, std::string> result;
    boost::filesystem::path filePathObj(dirPath);

    if(exists(dirPath) == false)
    {
        result.first = false;
        result.second = "path doesn't exist: " + dirPath;
        return result;
    }

    if(is_directory(dirPath))
    {
        result.first = false;
        result.second = "path exist, but is a file: " + dirPath;
        return result;
    }

    result.first = true;
    result.second = "";
    return result;
}

}
