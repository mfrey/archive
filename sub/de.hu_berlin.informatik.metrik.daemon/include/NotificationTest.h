#ifndef _NOTIFICATION_TEST_H_
#define _NOTIFICATION_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Notification.h" 

#include <stdlib.h>
#include <fstream>
#include <string>
#include <time.h>
#include <sstream>

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

class NotificationTest : public CppUnit::TestFixture {
  private:
    CPPUNIT_TEST_SUITE(NotificationTest);
    CPPUNIT_TEST(initialize);
    CPPUNIT_TEST(testAddRemove);
    CPPUNIT_TEST(testAddRemoveWatchDescriptorEntry);
    CPPUNIT_TEST_SUITE_END();

    Notification *a, *b;
    string mFileName;
    ofstream mFile;

    WatchDescriptorEntry createWatchDescriptorEntry(void);

  public:
    //
    void setUp(void);
    //
    void tearDown(void);
    // The method tests the add/remove method of class Notification
    void testAddRemove();
    // The method tests the add/remove method for an WatchDescriptorEntry object in class Notification
    void testAddRemoveWatchDescriptorEntry();
    //
    void initialize();
};

#endif
