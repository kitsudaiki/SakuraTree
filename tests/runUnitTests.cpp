/**
 *  @file    runUnitTests.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include "runUnitTests.h"

#include <tests/core/commonMethodsTest.h>

namespace SakuraTree
{

RunUnitTests::RunUnitTests()
{

}

void
RunUnitTests::run()
{
    #ifdef RUN_UNIT_TEST
    SakuraTree::CommonMethodsTest();
    #endif
}

}
