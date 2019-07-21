/**
 *  @file    sakuraRoot.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef SAKURAROOT_H
#define SAKURAROOT_H

#include <common.h>
#include <libKitsuneSakuraParser.h>

namespace Kitsune
{
namespace Jinja2
{
class KitsuneJinja2Converter;
}
}

using Kitsune::Sakura::LibKitsuneSakuraParser;
using Kitsune::Jinja2::KitsuneJinja2Converter;

namespace SakuraSeed
{
class SakuraTree;
class SakuraThread;
class BlossomTypeHandler;
struct BlossomData;

class SakuraRoot
{

public:
    SakuraRoot();
    ~SakuraRoot();

    bool startProcess(const std::string &rootPath,
                     std::string seedName="");

    void addMessage(BlossomData *blossomData);

    static SakuraSeed::SakuraRoot* m_root;
    static SakuraSeed::BlossomTypeHandler* m_typeHandler;
    static KitsuneJinja2Converter* m_jinja2Converter;

private:
    SakuraTree* m_rootTree = nullptr;
    SakuraThread* m_rootThread = nullptr;

    std::mutex m_mutex;
};

}

#endif // SAKURAROOT_H
