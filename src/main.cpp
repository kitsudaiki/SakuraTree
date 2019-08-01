/**
 *  @file    main.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include <common.h>
#include <sakuraRoot.h>
#include <tests/runUnitTests.h>
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
    ;

    argParser::variables_map vm;
    argParser::store(argParser::parse_command_line(argc, argv, desc), vm);
    argParser::notify(vm);

    // help-arg
    if(vm.count("help"))
    {
        std::cout << desc << std::endl;;
        return 0;
    }

    // seed-path-arg
    if(vm.count("seed-path"))
    {
        std::cout << "seed-path: "
                  << vm["seed-path"].as<std::string>()
                  << std::endl;
        SakuraTree::SakuraRoot* root = new SakuraTree::SakuraRoot();
        const std::string seedPath = vm["seed-path"].as<std::string>();
        root->startProcess(seedPath, "test_tree");
    }
    else
    {
        std::cout << "seed-path is missing"<<std::endl;
        return 1;
    }
    #endif

    return 0;
}
