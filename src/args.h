#ifndef ARGS_H
#define ARGS_H

#include <libKitsunemimiArgs/arg_parser.h>

namespace KyoukoMind
{

bool
registerArguments(Kitsunemimi::Args::ArgParser &argparser)
{
    argparser.registerPlain("debug,d",
                            "Enable debug-output.");

    argparser.registerPlain("use-config,c",
                            "Use a config-file as input.");

    argparser.registerString("item-input,i",
                             "Key-value-pairs to override the initial values inside of the file");


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
