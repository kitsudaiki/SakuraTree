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
            "init-tree",
            argParser::value<std::string>(),
            "path to the initial tree-file"
        )
        (
            "seed",
            argParser::value<std::string>(),
            "path to the seed-file"
        )
        (
            "input,i",
            argParser::value<std::vector<std::string>>()->multitoken()->zero_tokens()->composing(),
            "key-value-pairs to override the initial values inside of the file"
        )
        (
            "server-address",
            argParser::value<std::string>(),
            "address of the server"
        )
        (
            "server-port",
            argParser::value<uint16_t>(),
            "port of the server"
        )
        (
            "listen-address",
            argParser::value<std::string>(),
            "address where to listen"
        )
        (
            "listen-port",
            argParser::value<uint16_t>(),
            "port where to listen"
        )
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

    std::string initialTreePath = "";
    std::string seedPath = "";
    DataMap initialValues;
    std::string serverAddress = "";
    uint16_t serverPort = 0;
    std::string listenAddress = "";
    uint16_t listenPort = 0;

    // initial tree-file
    if(vm.count("init-tree"))
    {
        initialTreePath = vm["init-tree"].as<std::string>();
        std::cout << "init-tree: " << initialTreePath << std::endl;
    }

    // seed-file
    if(vm.count("seed"))
    {
        seedPath = vm["seed"].as<std::string>();
        std::cout << "seed: " << seedPath << std::endl;
    }

    // input-values
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

    // server-address
    if(vm.count("server-address"))
    {
        serverAddress = vm["server-address"].as<std::string>();
        std::cout << "server-address: " << serverAddress << std::endl;
    }

    // server-port
    if(vm.count("server-port"))
    {
        serverPort = vm["server-port"].as<uint16_t>();
        std::cout << "server-port: " << serverPort << std::endl;
    }

    // listen-address
    if(vm.count("listen-address"))
    {
        listenAddress = vm["listen-address"].as<std::string>();
        std::cout << "listen-address: " << listenAddress << std::endl;
    }

    // listen-port
    if(vm.count("listen-port"))
    {
        listenPort = vm["listen-port"].as<uint16_t>();
        std::cout << "listen-port: " << listenPort << std::endl;
    }

    if(vm.count("init-tree"))
    {
        SakuraTree::SakuraRoot* root = new SakuraTree::SakuraRoot(std::string(argv[0]));
        root->startProcess(initialTreePath,
                           seedPath,
                           initialValues,
                           listenAddress,
                           listenPort);
    }
    else if(vm.count("server-address")
            && vm.count("server-port"))
    {
        SakuraTree::SakuraRoot* root = new SakuraTree::SakuraRoot(std::string(argv[0]));
        root->startClientConnection(serverAddress,
                                    serverPort);

        while(true)
        {
            sleep(10);
        }
    }
    else
    {
        std::cout << "seed-path is missing"<<std::endl;
        return 1;
    }
    #endif

    return 0;
}
