#ifndef _TELNET_ACTOR_TEST_H_
#define _TELNET_ACTOR_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TelnetActor.h"

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

/**
 * 
 */
class TelnetActorTest : public CppUnit::TestFixture {
  private:
    CPPUNIT_TEST_SUITE(TelnetActorTest);
    CPPUNIT_TEST(testRun);
    CPPUNIT_TEST_SUITE_END();

    // Instances of the class which will be tested
    TelnetActor *t;

  public:
    /// The set up method of the unit test
    void setUp(void);
    /// The tear down method of the unit test
    void tearDown(void);
    /// The method tests the run() method of class TelnetActor
    void testRun(void);
};

#endif
