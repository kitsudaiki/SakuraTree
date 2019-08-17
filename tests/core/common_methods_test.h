/**
 *  @file    common_methods_test.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef COMMON_METHODS_TEST_H
#define COMMON_METHODS_TEST_H

#include <common.h>
#include <testing/test.h>
#include <common_methods.h>

namespace SakuraTree
{
class SakuraRoot;

class CommonMethodsTest : public Kitsune::Common::Test
{
public:
    CommonMethodsTest();

private:
    void initTestCase();
    void testConvertString();
    void testFillItems();
    void testOverrideItems();
    void testCheckItems();
    void cleanupTestCase();

    SakuraRoot* m_root = nullptr;
};

}

#endif // COMMON_METHODS_TEST_H
