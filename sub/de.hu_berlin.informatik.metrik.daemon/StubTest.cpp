#include "include/StubTest.h"

using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::StubTest);

LoggerPtr test::DequeTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.stubtest"));

void Stub::setUp(void){}

void Stub::tearDown(void){}
