#ifndef _WATCH_DESCRIPTOR_ENTRY_TEST_H_
#define _WATCH_DESCRIPTOR_ENTRY_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "WatchDescriptorEntry.h" 

#include <stdlib.h>

using namespace de::hu_berlin::informatik::metrik::daemon;

class WatchDescriptorEntryTest : public CppUnit::TestFixture {
  private:
    CPPUNIT_TEST_SUITE(WatchDescriptorEntryTest);
    CPPUNIT_TEST(initialize);
    CPPUNIT_TEST(testSetId);
    CPPUNIT_TEST(testGetId);
    CPPUNIT_TEST(testSetName);
    CPPUNIT_TEST(testGetName);
    CPPUNIT_TEST(testSetMask);
    CPPUNIT_TEST(testGetMask);
    CPPUNIT_TEST_SUITE_END();

    WatchDescriptorEntry *a, *b;

    // 
    static const int mId;
    //
    static const uint32_t mMask;
    //
    static const std::string mName;

  public:
    //
    void setUp(void);
    //
    void tearDown(void);
    //
    void testSetId();
    //
    void testGetId();
    //
    void testSetName();
    //
    void testGetName();
    //
    void testSetMask();
    //
    void testGetMask();
    //
    void initialize();
};

#endif
