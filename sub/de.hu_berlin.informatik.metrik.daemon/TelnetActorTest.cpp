#include "include/TelnetActorTest.h"

using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::TelnetActorTest);

LoggerPtr test::TelnetActorTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.telnetactortest"));

void test::TelnetActorTest::setUp(){
  t = new TelnetActor("localhost","4519");
}

void test::TelnetActorTest::tearDown(){
  delete t;
}

void test::TelnetActorTest::testRun(){
  CPPUNIT_ASSERT(t != NULL);

  //t.run();
}

