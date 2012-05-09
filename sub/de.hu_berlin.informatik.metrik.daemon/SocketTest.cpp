#include "include/SocketTest.h"

using namespace std;
using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::SocketTest);

LoggerPtr test::SocketTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.sockettest"));

void test::SocketTest::setUp(void){}

void test::SocketTest::tearDown(void){}
