#ifndef PROCESS_HANDLING_H
#define PROCESS_HANDLING_H

#include <common.h>
#include <items/sakura_items.h>

namespace SakuraTree
{

bool runSyncProcess(BlossomItem &blossomItem,
                    std::string command);
bool runSyncProcess(BlossomItem &blossomItem,
                    const std::string &programm,
                    const std::vector<std::string> &args);

}

#endif // PROCESS_HANDLING_H
