#include "include/TelnetActorTest.h"

using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::TelnetActorTest);

LoggerPtr test::TelnetActorTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.telnetactortest"));

void test::TelnetActorTest::setUp(){
//  t = new TelnetActor("localhost","4519");
// TODO: Find a suitable public telnet service or start a minimal telnet daemon
  t = new TelnetActor("nethack.alt.org","23");
}

void test::TelnetActorTest::tearDown(){
  delete t;
}

void test::TelnetActorTest::testRun(){
  LOG4CXX_TRACE(mLogger, "run thread");
  mActor = boost::thread(&test::TelnetActorTest::telnetActorThread, this); 
  // Create a writer thread
  mWriter = boost::thread(&test::TelnetActorTest::writeDataThread, this); 
  mActor.join();
  mWriter.join();
  LOG4CXX_TRACE(mLogger, "writer thread finished");
}

void test::TelnetActorTest::telnetActorThread(void){
  // Initialize thread in telnet actor class
  t->run();
  //
  LOG4CXX_TRACE(mLogger, "create writer thread");
}

void test::TelnetActorTest::writeDataThread(void){
  // Run the thread in class telnet actor
  /*
  LOG4CXX_TRACE(mLogger, "send w");
  t->send("w");
  LOG4CXX_TRACE(mLogger, "send q");
  t->send("q");
  LOG4CXX_TRACE(mLogger, "send q");
  t->send("q");
  LOG4CXX_TRACE(mLogger, "send #stop");
  t->send("#stop");
  */
}

void test::TelnetActorTest::testRecv(){

}

void test::TelnetActorTest::testSend(){

}
