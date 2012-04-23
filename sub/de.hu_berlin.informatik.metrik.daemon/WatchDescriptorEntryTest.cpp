#include "include/WatchDescriptorEntryTest.h" 

using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::WatchDescriptorEntryTest);

const int test::WatchDescriptorEntryTest::mId = 42;
const uint32_t test::WatchDescriptorEntryTest::mMask = 23;
const std::string test::WatchDescriptorEntryTest::mName("WatchDescriptorEntryTest");

LoggerPtr test::WatchDescriptorEntryTest::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.watchdescriptorentrytest"));

void test::WatchDescriptorEntryTest::setUp(){
  a = new WatchDescriptorEntry();
  b = new WatchDescriptorEntry(mId, mMask, mName);
}

void test::WatchDescriptorEntryTest::tearDown(){
  delete a; delete b;
}

void test::WatchDescriptorEntryTest::testGetId(){
  LOG4CXX_TRACE(logger, "get id");
  CPPUNIT_ASSERT(b != NULL);
  CPPUNIT_ASSERT(b->getId() == mId);
}

void test::WatchDescriptorEntryTest::testSetId(){
  LOG4CXX_TRACE(logger, "set id");
  CPPUNIT_ASSERT(a != NULL);
  a->setId(mId);
  CPPUNIT_ASSERT(a->getId() == mId);
}

void test::WatchDescriptorEntryTest::testGetName(){
  LOG4CXX_TRACE(logger, "get name");
  CPPUNIT_ASSERT(b != NULL);
  CPPUNIT_ASSERT(b->getName() == mName);
}

void test::WatchDescriptorEntryTest::testSetName(){
  LOG4CXX_TRACE(logger, "set name");
  CPPUNIT_ASSERT(a != NULL);
  a->setName(mName);
  CPPUNIT_ASSERT(((a->getName()).compare(mName)) == 0);
}

void test::WatchDescriptorEntryTest::testGetMask(){
  LOG4CXX_TRACE(logger, "get mask");
  CPPUNIT_ASSERT(b != NULL);
  CPPUNIT_ASSERT(b->getMask() == mMask);
}

void test::WatchDescriptorEntryTest::testSetMask(){
  LOG4CXX_TRACE(logger, "set mask");
  CPPUNIT_ASSERT(a != NULL);
  a->setMask(mMask);
  CPPUNIT_ASSERT(a->getMask() == mMask);
}
