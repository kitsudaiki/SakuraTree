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
#include <sakura_root.h>
#include <tests/run_unit_tests.h>
#include <boost/program_options.hpp>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

namespace argParser = boost::program_options;

int main(int argc, char *argv[])
{
    // run unit-tests, if enabled by define-value
    #ifdef RUN_UNIT_TEST
    SakuraTree::RunUnitTests unitTests;
    unitTests.run();
    #else

    // Declare the supported options.
    argParser::options_description desc("Allowed options");
    desc.add_options()
        (
            "help,h", // -h and --help for help-text
            "produce help message"
        )
        (
            "seed-path",
            argParser::value<std::string>(),
            "path to the initial seed-file"
        )
        /*(
            "seed-name",
            argParser::value<std::string>(),
            "set name initial seed-file"
        )*/
        (
            "input,i",
            argParser::value<std::vector<std::string>>()->multitoken()->zero_tokens()->composing(),
            "key-value-pairs to override the initial values inside of the file"
        )
        // TODO: enable again in 0.3.0
        /*(
            "server-address",
            argParser::value<std::string>(),
            "address of the server"
        )
        (
            "server-port",
            argParser::value<int>(),
            "port of the server"
        )*/
    ;

    argParser::variables_map vm;
    argParser::store(argParser::parse_command_line(argc, argv, desc), vm);
    argParser::notify(vm);

    // help-arg
    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    // seed-path-arg
    if(vm.count("seed-path"))
    {
        std::cout << "seed-path: "
                  << vm["seed-path"].as<std::string>()
                  << std::endl;
        SakuraTree::SakuraRoot* root = new SakuraTree::SakuraRoot(std::string(argv[0]));

        // seed-path
        const std::string seedPath = vm["seed-path"].as<std::string>();

        // seed-name
        std::string seedName = "";
        if(vm.count("seed-name")) {
            seedName = vm["seed-name"].as<std::string>();
        }

        // input-values
        DataMap initialValues;
        if(vm.count("input"))
        {
            std::vector<std::string> envs = vm["input"].as<std::vector<std::string>>();
            for(uint32_t i = 0; i < envs.size(); i++)
            {
                std::vector<std::string> pair;
                Kitsunemimi::splitStringByDelimiter(pair, envs.at(i), '=');
                if(pair.size() != 2)
                {
                    std::cout << "'"<<envs.at(i)<<"' is not a valid pair"<<std::endl;
                    return 1;
                }
                initialValues.insert(pair.at(0), new DataValue(pair.at(1)));
            }
        }

        // start
        root->startProcess(seedPath, seedName, initialValues);
    }
    // TODO: enable again in 0.3.0
    /*else if(vm.count("server-address") && vm.count("server-port"))
    {
        SakuraTree::SakuraRoot* root = new SakuraTree::SakuraRoot(std::string(argv[0]));
        const std::string address = vm["server-address"].as<std::string>();
        const int port = vm["server-port"].as<int>();

        root->startClientConnection(address, port);

        while(true)
        {
            sleep(10);
        }
    }*/
    else
    {
        std::cout << "seed-path is missing"<<std::endl;
        return 1;
    }
    #endif

    return 0;
}
