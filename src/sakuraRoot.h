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
#include <sakura_converter.hpp>

namespace Kitsune
{
namespace Jinja2
{
class Jinja2Converter;
}
}

using Kitsune::Sakura::SakuraConverter;
using Kitsune::Jinja2::Jinja2Converter;

namespace SakuraTree
{
class SakuraThread;
struct BlossomData;

class SakuraRoot
{

public:
    SakuraRoot();
    ~SakuraRoot();

    bool startProcess(const std::string &rootPath,
                     std::string seedName="");

    void addMessage(BlossomData *blossomData);

    static SakuraTree::SakuraRoot* m_root;
    static Jinja2Converter* m_jinja2Converter;

private:
    SakuraThread* m_rootThread = nullptr;

    std::mutex m_mutex;
};

}

#endif // SAKURAROOT_H
