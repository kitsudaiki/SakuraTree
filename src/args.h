#ifndef ARGS_H
#define ARGS_H

#include <libKitsunemimiArgs/arg_parser.h>

namespace KyoukoMind
{

bool
registerArguments(Kitsunemimi::Args::ArgParser &argparser)
{
    argparser.registerString("directory-path,d",
                             "path to directory with all tree-files");

    argparser.registerString("init-tree-id,t",
                             "id of the initial tree-file within the choosen directory");

    argparser.registerString("init-tree",
                             "path a single tree-file");

    argparser.registerString("seed",
                             "path to the seed-file");

    argparser.registerString("input,i",
                             "key-value-pairs to override the initial values inside of the file");

    // connect to server
    argparser.registerString("server-address",
                             "address of the server");

    argparser.registerInteger("server-port",
                              "port of the server");

    // listen
    argparser.registerString("listen-address",
                             "address where to listen for incoming connections");

    argparser.registerInteger("listen-port",
                             "port where to listen for incoming connections");

    return true;
}

}

#endif // ARGS_H
