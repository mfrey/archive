#ifndef _STUB_TEST_H_
#define _STUB_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

/**
 * THIS IS A STUB: PLEASE FILL ME WITH LIFE.
 */
class StubTest : public CppUnit::TestFixture {
  private:
    CPPUNIT_TEST_SUITE(NotificationTest);
    // CPPUNIT_TEST(testAddRemove);
    CPPUNIT_TEST_SUITE_END();

    // Instances of the class which will be tested
    //Notification *a, *b;

  public:
    /// The set up method of the unit test
    void setUp(void);
    /// The tear down method of the unit test
    void tearDown(void);
};

#endif
