/**
 * @file        path_copy_blossom.h
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

#ifndef PATH_COPY_BLOSSOM_H
#define PATH_COPY_BLOSSOM_H

#include <libKitsunemimiSakuraLang/blossom.h>

class PathCopyBlossom_Test;

class PathCopyBlossom
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PathCopyBlossom();

    Kitsunemimi::Sakura::Blossom* createNewInstance();

protected:
    void initBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem);
    void preCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem);
    void runTask(Kitsunemimi::Sakura::BlossomItem &blossomItem);
    void postCheck(Kitsunemimi::Sakura::BlossomItem &blossomItem);
    void closeBlossom(Kitsunemimi::Sakura::BlossomItem &blossomItem);

private:
    friend PathCopyBlossom_Test;

    std::string m_sourcePath = "";
    std::string m_destinationPath = "";
    std::string m_mode = "";
    std::string m_owner = "";
    bool m_localStorage = false;
};

#endif // PATH_COPY_BLOSSOM_H
