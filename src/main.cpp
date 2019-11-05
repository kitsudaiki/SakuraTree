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

namespace argParser = boost::program_options;

int main(int argc, char *argv[])
{
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
            "set the path to the initial seed-file"
        )
        (
            "seed-name",
            argParser::value<std::string>(),
            "set the path to the initial seed-file"
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

    // seed-path-arg
    if(vm.count("seed-path") && vm.count("seed-name"))
    {
        std::cout << "seed-path: "
                  << vm["seed-path"].as<std::string>()
                  << std::endl;
        SakuraTree::SakuraRoot* root = new SakuraTree::SakuraRoot(std::string(argv[0]));
        const std::string seedPath = vm["seed-path"].as<std::string>();
        root->startProcess(seedPath, vm["seed-name"].as<std::string>());
    }
    else
    {
        std::cout << "seed-path is missing"<<std::endl;
        return 1;
    }
    #endif

    return 0;
}
