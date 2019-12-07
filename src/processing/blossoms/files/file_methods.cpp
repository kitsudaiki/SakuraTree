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
bool
doesPathExist(const std::string path)
{
    return boost::filesystem::exists(path);
}

/**
 * @brief doesFileExist
 * @param filePath
 * @return
 */
bool
doesFileExist(const std::string filePath)
{
    if(boost::filesystem::exists(filePath) == false
            || boost::filesystem::is_directory(filePath))
    {
        return false;
    }

    return true;
}

/**
 * @brief doesDirExist
 * @param dirPath
 * @return
 */
bool
doesDirExist(const std::string dirPath)
{
    if(boost::filesystem::exists(dirPath) == false
            || boost::filesystem::is_directory(dirPath) == false)
    {
        return false;
    }

    return true;
}

/**
 * @brief renameFileOrDir
 * @param oldPath
 * @param newPath
 * @return
 */
const std::pair<bool, std::string>
renameFileOrDir(const std::string oldPath,
                const std::string newPath)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    boost::filesystem::rename(oldPath, newPath, error);

    if(error.value() != 0)
    {
        result.first = false;
        result.second = error.message();
        return result;
    }

    result.first = true;
    result.second = "";
    return result;
}

/**
 * @brief copyPath
 * @param sourcePath
 * @param targetPath
 * @return
 */
const std::pair<bool, std::string>
copyPath(const std::string sourcePath, const std::string targetPath)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    boost::filesystem::remove(targetPath);
    boost::filesystem::copy(sourcePath, targetPath, error);

    if(error.value() != 0)
    {
        result.first = false;
        result.second = error.message();
        return result;
    }

    result.first = true;
    result.second = "";
    return result;
}

/**
 * @brief deleteFileOrDis
 * @param path
 * @return
 */
bool
deleteFileOrDir(const std::string path)
{
    return boost::filesystem::remove(path);
}

}
