#ifndef _NOTIFICATION_TEST_H_
#define _NOTIFICATION_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Notification.h" 

#include <stdlib.h>

using namespace de::hu_berlin::informatik::metrik::daemon;

class NotificationTest : public CppUnit::TestFixture {
  private:
    CPPUNIT_TEST_SUITE(NotificationTest);
    CPPUNIT_TEST(initialize);
    CPPUNIT_TEST(testAdd);
    CPPUNIT_TEST_SUITE_END();

    Notification *a, *b;

  public:
    //
    void setUp(void);
    //
    void tearDown(void);
    //
    void testAdd();
    //
    void initialize();
};

#endif
