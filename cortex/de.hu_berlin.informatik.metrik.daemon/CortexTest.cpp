#include "include/CortexTest.h"

using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::CortexTest);

LoggerPtr test::CortexTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.cortextest"));

void Stub::setUp(void){}

void Stub::tearDown(void){}
