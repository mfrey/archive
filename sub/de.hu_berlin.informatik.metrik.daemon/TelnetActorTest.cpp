#include "include/TelnetActorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TelnetActorTest);

void TelnetActorTest::setUp(){
  t = new TelnetActor("localhost","4519");
}

void TelnetActorTest::tearDown(){
  delete t;
}

void TelnetActorTest::testRun(){
  CPPUNIT_ASSERT(t != NULL);

  //t.run();

}
