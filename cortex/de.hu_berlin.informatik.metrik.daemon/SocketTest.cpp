#include "include/SocketTest.h"

using namespace std;
using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::SocketTest);

LoggerPtr test::SocketTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.sockettest"));

void test::SocketTest::setUp(void){}

void test::SocketTest::tearDown(void){}

void test::SocketTest::testOpen(){
  ///
  s = new Socket(string("nethack.alt.org"), string("23"));
  ///
  int result = s->open();
  LOG4CXX_TRACE(mLogger, "result of operation open() is " << result);
  ///
  CPPUNIT_ASSERT(result == 0);
  ///
  if(result == 0){
    s->closeSocket();
  }

}
