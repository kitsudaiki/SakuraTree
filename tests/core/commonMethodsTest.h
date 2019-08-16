/**
 *  @file    commonMethodsTest.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef COMMONMETHODSTEST_H
#define COMMONMETHODSTEST_H

#include <common.h>
#include <testing/test.hpp>
#include <commonMethods.h>

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

#endif // COMMONMETHODSTEST_H
