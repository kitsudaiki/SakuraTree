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

    argparser.registerPlain("version,v",
                            "Print version number.");

    argparser.registerString("item-input,i",
                             "Key-value-pairs to override the initial values inside of the file");

    argparser.registerPlain("dry-run",
                            "Try to parse and validate all file without executing the scripts");

    // required input
    argparser.registerString("input-path",
                             "Relative or absolut path to the initial sakura-file or to the "
                             "directory, which contains the sakura.root file, which should be "
                             "executed.",
                             true,
                             true);

    return true;
}

}

#endif // ARGS_H
