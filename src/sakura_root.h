/**
 *  @file    sakura_root.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURA_ROOT_H
#define SAKURA_ROOT_H

#include <common.h>
#include <libKitsunemimiSakuraParser/sakura_converter.h>

namespace Kitsunemimi
{
namespace Jinja2
{
class Jinja2Converter;
}
}

using Kitsunemimi::Sakura::SakuraConverter;
using Kitsunemimi::Jinja2::Jinja2Converter;

namespace SakuraTree
{
class SakuraThread;
struct BlossomItem;

class SakuraRoot
{

public:
    SakuraRoot(const std::string &executablePath);
    ~SakuraRoot();

    bool startProcess(const std::string &rootPath,
                     std::string seedName="");

    void addMessage(const BlossomItem &blossomItem);

    static SakuraTree::SakuraRoot* m_root;
    static Jinja2Converter* m_jinja2Converter;
    static std::string m_executablePath;

private:
    SakuraThread* m_rootThread = nullptr;

    std::mutex m_mutex;
};

}

#endif // SAKURA_ROOT_H
