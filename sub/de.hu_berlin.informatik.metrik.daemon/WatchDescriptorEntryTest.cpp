#include "include/WatchDescriptorEntryTest.h" 

CPPUNIT_TEST_SUITE_REGISTRATION(WatchDescriptorEntryTest);

const int WatchDescriptorEntryTest::mId = 42;
const uint32_t WatchDescriptorEntryTest::mMask = 23;
const std::string WatchDescriptorEntryTest::mName("WatchDescriptorEntryTest");

void WatchDescriptorEntryTest::setUp(){
  a = new WatchDescriptorEntry();
  b = new WatchDescriptorEntry(mId, mMask, mName);
}

void WatchDescriptorEntryTest::tearDown(){
  delete a; delete b;
}

void WatchDescriptorEntryTest::testGetId(){
  CPPUNIT_ASSERT(b != NULL);
  CPPUNIT_ASSERT(b->getId() == mId);
}

void WatchDescriptorEntryTest::testSetId(){
  CPPUNIT_ASSERT(a != NULL);
  a->setId(mId);
  CPPUNIT_ASSERT(a->getId() == mId);
}

void WatchDescriptorEntryTest::testGetName(){
  CPPUNIT_ASSERT(b != NULL);
  CPPUNIT_ASSERT(b->getName() == mName);
}

void WatchDescriptorEntryTest::testSetName(){
  CPPUNIT_ASSERT(a != NULL);
  a->setName(mName);
  CPPUNIT_ASSERT(((a->getName()).compare(mName)) == 0);
}

void WatchDescriptorEntryTest::testGetMask(){
  CPPUNIT_ASSERT(b != NULL);
  CPPUNIT_ASSERT(b->getMask() == mMask);
}

void WatchDescriptorEntryTest::testSetMask(){
  CPPUNIT_ASSERT(a != NULL);
  a->setMask(mMask);
  CPPUNIT_ASSERT(a->getMask() == mMask);
}

/*
void WatchDescriptorEntryTest::init(){

}
*/

void WatchDescriptorEntryTest::initialize(){

}

