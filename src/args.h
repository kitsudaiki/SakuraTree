#ifndef ARGS_H
#define ARGS_H

#include <libKitsunemimiArgs/arg_parser.h>

namespace KyoukoMind
{

bool
registerArguments(Kitsunemimi::Args::ArgParser &argparser)
{
    argparser.registerString("init-tree-id,t",
                             "Id of the initial tree-file within the choosen directory");

    argparser.registerString("seed-path,s",
                             "Path to the seed-file");

    argparser.registerString("item-input,i",
                             "Key-value-pairs to override the initial values inside of the file");

    // connect to server
    argparser.registerString("server-address",
                             "Address of the server");

    argparser.registerInteger("server-port",
                              "Port of the server");

    // listen
    argparser.registerString("listen-address",
                             "Address where to listen for incoming connections");

    argparser.registerInteger("listen-port",
                             "Port where to listen for incoming connections");

    // required input
    argparser.registerString("input-path",
                             "Directory- or file-path for the tree-input. If this path is a "
                             "directory, the argument init-tree-id is also required to identify "
                             "the tree inside of the directory.",
                             true,
                             true);

    return true;
}

}

#endif // ARGS_H