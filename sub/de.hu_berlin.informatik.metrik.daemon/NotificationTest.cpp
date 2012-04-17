#include "include/NotificationTest.h" 

CPPUNIT_TEST_SUITE_REGISTRATION(NotificationTest);

void NotificationTest::setUp(){
  a = new Notification();
}

void NotificationTest::tearDown(){
  delete a; delete b;
}

void NotificationTest::testAdd(){
  CPPUNIT_ASSERT(a != NULL);
  a->add("/proc/loadavg", IN_MODIFY);
  CPPUNIT_ASSERT();
}  


void NotificationTest::initialize(){

}

