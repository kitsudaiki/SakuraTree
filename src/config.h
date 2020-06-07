#ifndef CONFIG_H
#define CONFIG_H

#include <common.h>
#include <libKitsunemimiConfig/config_handler.h>

namespace SakuraTree
{

void
registerConfigs()
{
    assert(REGISTER_INT_CONFIG("server", "server_port", 1337));
    assert(REGISTER_STRING_CONFIG("server", "server_address", "127.0.0.1"));
}

}

#endif // CONFIG_H
