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
    return exists(path);
}

/**
 * @brief doesFileExist
 * @param filePath
 * @return
 */
bool
doesFileExist(const std::string filePath)
{
    if(exists(filePath) == false
            && is_directory(filePath))
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
    if(exists(dirPath)
            && is_directory(dirPath))
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
std::pair<bool, std::string>
renameFileOrDir(const std::string oldPath,
                const std::string newPath)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    rename(oldPath, newPath, error);

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
 * @brief copyFile
 * @param sourcePath
 * @param targetPath
 * @return
 */
std::pair<bool, std::string>
copyFile(const std::string sourcePath, const std::string targetPath)
{
    std::pair<bool, std::string> result;

    boost::system::error_code error;
    error.clear();

    copy_file(sourcePath, targetPath, error);

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
deleteFileOrDis(const std::string path)
{
    return remove(path);
}

}
