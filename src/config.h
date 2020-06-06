#ifndef CONFIG_H
#define CONFIG_H

#include <libKitsunemimiConfig/config_handler.h>

namespace SakuraTree
{

void
registerConfigs()
{
    REGISTER_INT_CONFIG("Server", "port", 1337);
    REGISTER_STRING_CONFIG("Server", "address", "127.0.0.1");
}

}

#endif // CONFIG_H
