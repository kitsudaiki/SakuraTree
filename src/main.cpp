/**
 * @file        main.cpp
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

#include <common.h>
#include <args.h>
#include <sakura_root.h>

#include <libKitsunemimiCommon/common_methods/string_methods.h>
#include <libKitsunemimiPersistence/logger/logger.h>
#include <libKitsunemimiPersistence/files/file_methods.h>
#include <libKitsunemimiArgs/arg_parser.h>

int main(int argc, char *argv[])
{
    Kitsunemimi::Persistence::initConsoleLogger(false);

    // create and init argument-parser
    Kitsunemimi::Args::ArgParser argParser;
    KyoukoMind::registerArguments(argParser);

    // parse cli-input
    if(argParser.parse(argc, argv) == false) {
        return 1;
    }

    bool enableDebug = false;
    DataMap itemInputValues;

    std::string listenAddress = "";
    uint16_t listenPort = 0;

    // check debug-output
    if(argParser.wasSet("debug"))
    {
        enableDebug = true;
        Kitsunemimi::Persistence::setDebugFlag(true);
    }

    // input-values
    if(argParser.wasSet("item-input"))
    {
        std::vector<std::string> itemInput = argParser.getStringValues("item-input");
        for(const std::string& item : itemInput)
        {
            std::vector<std::string> pair;
            Kitsunemimi::splitStringByDelimiter(pair, item, '=');
            if(pair.size() != 2)
            {
                std::cout << "'" << item << "' is not a valid pair" << std::endl;
                return 1;
            }
            itemInputValues.insert(pair.at(0), new DataValue(pair.at(1)));
        }
    }

    // listen-address
    if(argParser.wasSet("listen-address"))
    {
        listenAddress = argParser.getStringValues("listen-address")[0];
        std::cout << "listen-address: " << listenAddress << std::endl;
    }

    // listen-port
    if(argParser.wasSet("listen-port"))
    {
        listenPort = static_cast<uint16_t>(argParser.getIntValues("listen-port")[0]);
        std::cout << "listen-port: " << listenPort << std::endl;
    }

    // input-path
    bfs::path inputPath = argParser.getStringValues("input-path")[0];
    std::cout << "input-path: " << inputPath << std::endl;
    if(inputPath.is_relative()) {
        inputPath = bfs::absolute(inputPath);
    }
    std::cout<<"absolute input-path: "<<inputPath<<std::endl;

    SakuraRoot* root = new SakuraRoot(std::string(argv[0]));

    if(root->startProcess(inputPath.string(),
                          itemInputValues,
                          enableDebug,
                          argParser.wasSet("dry-run")))
    {
        return 1;
    }

    return 0;
}
