#include "include/DequeTest.h"

using namespace log4cxx;

CPPUNIT_TEST_SUITE_REGISTRATION(test::DequeTest);

LoggerPtr test::DequeTest::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.test.dequetest"));

void test::DequeTest::setUp(void){
  d = new Deque<std::string>();
}

void test::DequeTest::tearDown(void){
  delete d;
}

/**
 * This method tests the pushFront() method of the class Deque. A thread is 
 * initialized which simply pushes a quote from shakespeare to the data 
 * structure.(at front).
 */
void test::DequeTest::pushFrontTest(){
  LOG4CXX_TRACE(mLogger, "start unit test pushFrontTest()");
  CPPUNIT_ASSERT(d->size() == 0);
  LOG4CXX_TRACE(mLogger, "create writer thread (pushFrontTestWriterThread) which pushes strings up front");
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushFrontThread, this); 
  LOG4CXX_TRACE(mLogger, "waiting for thread to finish");
  // Wait for writer thread to finish
  mWriter.join();
  // Deque should contain 26 elements
  CPPUNIT_ASSERT(d->size() == 26);
  LOG4CXX_TRACE(mLogger, "stop unit test pushFrontTest()");
}

/**
 * This method tests the pushBack() method of the class Deque. A thread is 
 * initialized which simply pushes a quote from shakespear to the data 
 * structure (at back).
 */
void test::DequeTest::pushBackTest(){
  LOG4CXX_TRACE(mLogger, "start unit test pushBackTest()");
  CPPUNIT_ASSERT(d->size() == 0);
  LOG4CXX_TRACE(mLogger, "create thread (pushBackTestWriterThread) which pushes data up back");
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushBackThread, this); 
  LOG4CXX_TRACE(mLogger, "wait for thread (pushBackTestWriterThread) to finish");
  // Wait for writer thread to finish
  mWriter.join();
  // Deque should contain 14 elements
  CPPUNIT_ASSERT(d->size() == 14);
  LOG4CXX_TRACE(mLogger, "stop unit test pushBackTest()");
}

/**
 * This method tests the pushFront() and waitAndPopFront() methods of the 
 * clss Deque. Here, a reader and writer thread are initialized which write
 * (pushFront) and read/remvove (waitAndPopFront) data from a deque.
 */
void test::DequeTest::pushFrontReaderWriterTest(){
  LOG4CXX_TRACE(mLogger, "start unit test pushFronReaderWriterTest()");
  CPPUNIT_ASSERT(d->size() == 0);
  LOG4CXX_TRACE(mLogger, "create thread (pushFrontTestReaderThread) which reads via pop_front from deque");
  // Create the reader thread
  mReader = boost::thread(&test::DequeTest::popBackThread, this); 
  LOG4CXX_TRACE(mLogger, "create thread (pushFrontTestWriterThread) which writes via push_front from deque");
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushFrontThread, this); 
  LOG4CXX_TRACE(mLogger, "waiting for thread (pushFrontReaderTestThread) to finish");
  // Wait for reader thread to finish
  mReader.join(); 
  LOG4CXX_TRACE(mLogger, "waiting for thread (pushFrontWriterTestThread) to finish");
  // Wait for writer thread to finish
  mWriter.join();
  LOG4CXX_TRACE(mLogger, "size of deque is " << d->size());
  CPPUNIT_ASSERT(d->size() == 0);
  LOG4CXX_TRACE(mLogger, "stop unit test pushFronReaderWriterTest()");
}

/**
 * This method tests the pushBack() and waitAndPopBack() methods of the 
 * clss Deque. Here, a reader and writer thread are initialized which write
 * (pushFront) and read/remvove (waitAndPopBack) data from a deque.
 */
void test::DequeTest::pushBackReaderWriterTest(){
  LOG4CXX_TRACE(mLogger, "start pushBackReaderWriterThread()");
  CPPUNIT_ASSERT(d->size() == 0);
  LOG4CXX_TRACE(mLogger, "create thread (pushBackTestReaderThread) which reads data via pop_back");
  // Create the reader thread
  mReader = boost::thread(&test::DequeTest::popFrontThread, this); 
  LOG4CXX_TRACE(mLogger, "create thread (pushBackTestWriterThread) which writes data via push_back");
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushBackThread, this); 
  LOG4CXX_TRACE(mLogger, "wait for thread (pushBackThread) to finish");
  // Wait for reader thread to finish
  mReader.join(); 
  LOG4CXX_TRACE(mLogger, "wait for thread (popFrontThread) to finish");
  // Wait for writer thread to finish
  mWriter.join();
  LOG4CXX_TRACE(mLogger, "size of deque is " << d->size());
  CPPUNIT_ASSERT(d->size() == 0);
  LOG4CXX_TRACE(mLogger, "stop pushBackReaderWriterThread()");
}

/*
void test::DequeTest::multipleReaderMultipleWriteTest(){

}
*/

std::vector<std::string> test::DequeTest::splitString(std::string pString){
  // Create an array which holds a splitted citation
  std::vector<std::string> array;
  // Split a string 
  boost::split(array, pString, boost::is_space());
  // Return the result
  return array;
}

void test::DequeTest::pushFrontThread(){
  LOG4CXX_TRACE(mLogger, "start pushFrontTestWriterThread()");
  // Create an array which holds a splitted citation
  std::vector<std::string> array = splitString("We few, we happy few, we band of brothers. For he today that sheds his blood with me shall be my brother; be never so vile.");
  // Write the citation into the deque
 for(unsigned int i = 0; i < array.size(); i++){
    LOG4CXX_TRACE(mLogger, "push_front " << array[i]);
    // Push each entry into the deque
    d->pushFront(array[i]);
  }
  LOG4CXX_TRACE(mLogger, "stop pushFrontTestWriterThread()");
}

void test::DequeTest::pushBackThread(){
  LOG4CXX_TRACE(mLogger, "start pushBackTestWriterThread()");
  // Create an array which holds a splitted citation
  std::vector<std::string> array = splitString("My hands are of your colour; but I shame/To wear a heart so white");
  // Write the citation into the deque
 for(unsigned int i = 0; i < array.size(); i++){
    LOG4CXX_TRACE(mLogger, "push_back " << array[i]);
    // Push each entry into the deque
    d->pushBack(array[i]);
  }
  LOG4CXX_TRACE(mLogger, "stop pushBackTestWriterThread()");
}

void test::DequeTest::popFrontThread(){
  LOG4CXX_TRACE(mLogger, "start popFrontThread()");
  while(true){
    // String to store the result from the waitAndPopBack() operation
    std::string result;
    // Read data from deque
    d->waitAndPopFront(result);
    LOG4CXX_TRACE(mLogger, "wait and pop_front " << result);
    // Stop if last entry was read (which should be 'vile.')
    if(result.compare("white") == 0){
      LOG4CXX_TRACE(mLogger, "read control sign, will break execution");
      break;
    }
  }
  LOG4CXX_TRACE(mLogger, "stop pushFrontTestReaderThread()");
}



void test::DequeTest::popBackThread(){
  LOG4CXX_TRACE(mLogger, "start pushBackTestReaderThread()");
  while(true){
    // String to store the result from the waitAndPopBack() operation
    std::string result;
    // Read data from deque
    d->waitAndPopBack(result);
    LOG4CXX_TRACE(mLogger, "wait and pop_back " << result);

    // Stop if last entry was read (which should be 'vile.')
    if(result.compare("vile.") == 0){
      LOG4CXX_TRACE(mLogger, "read control sign, will break execution");
      break;
    }
  }
  LOG4CXX_TRACE(mLogger, "stop pushBackTestReaderThread()");
}
