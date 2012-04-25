#include "include/DequeTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(test::DequeTest);

void test::DequeTest::setUp(void){
  d = new Deque<std::string>();
}

void test::DequeTest::tearDown(void){
  delete d;
}

/**
 * This method tests the pushFront() method of the class Deque. A thread is 
 * initialized which simply pushes a quote from shakespear to the data 
 * structure.
 */
void test::DequeTest::pushFrontTest(){
  CPPUNIT_ASSERT(d->size() == 0);
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushFrontTestWriterThread, this); 
  // Wait for writer thread to finish
  mWriter.join();
  // Deque should contain 27 elements
  CPPUNIT_ASSERT(d->size() == 27);
}

/**
 * This method tests the pushBack() method of the class Deque. A thread is 
 * initialized which simply pushes a quote from shakespear to the data 
 * structure.
 */
void test::DequeTest::pushBackTest(){
  CPPUNIT_ASSERT(d->size() == 0);
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushBackTestWriterThread, this); 
  // Wait for writer thread to finish
  mWriter.join();
  // Deque should contain 14 elements
  CPPUNIT_ASSERT(d->size() == 14);
}


void test::DequeTest::pushFrontReaderWriterTest(){
  CPPUNIT_ASSERT(d->size() == 0);
  // Create the reader thread
  mReader = boost::thread(&test::DequeTest::pushFrontTestReaderThread, this); 
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushFrontTestWriterThread, this); 
  // Wait for reader thread to finish
  mReader.join(); 
  // Wait for writer thread to finish
  mWriter.join();
  std::cout << " size is " << d->size() << std::endl;
  CPPUNIT_ASSERT(d->size() == 0);
}

void test::DequeTest::pushBackReaderWriterTest(){
  CPPUNIT_ASSERT(d->size() == 0);
  // Create the reader thread
  mReader = boost::thread(&test::DequeTest::pushBackTestReaderThread, this); 
  // Create the writer thread
  mWriter = boost::thread(&test::DequeTest::pushBackTestWriterThread, this); 
  // Wait for reader thread to finish
  mReader.join(); 
  // Wait for writer thread to finish
  mWriter.join();
  std::cout << " size is " << d->size() << std::endl;
  CPPUNIT_ASSERT(d->size() == 0);
}
/*
void test::DequeTest::multipleReaderMultipleWriteTest(){

}
*/

void test::DequeTest::pushFrontTestWriterThread(){
  // Create an array which holds a splitted citation
  std::vector<std::string> array;
  // Split a citation from Shakespears Henry V 
  boost::split(array,"# We few, we happy few, we band of brothers. For he today that sheds his blood with me shall be my brother; be never so vile.", boost::is_space());
  // Write the citation into the deque
 for(unsigned int i = 0; i < array.size(); i++){
    // Push each entry into the deque
    d->pushFront(array[i]);
  }
}

void test::DequeTest::pushBackTestWriterThread(){
  // Create an array which holds a splitted citation
  std::vector<std::string> array;
  // Split a citation from Shakespears MacBeth 
  boost::split(array,"My hands are of your colour; but I shame/To wear a heart so white", boost::is_space());
  // Write the citation into the deque
 for(unsigned int i = 0; i < array.size(); i++){
    // Push each entry into the deque
    d->pushBack(array[i]);
  }
}

void test::DequeTest::pushFrontTestReaderThread(){
  // Array for storing the result
  std::vector<std::string> array;

  while(true){
    // String to store the result from the waitAndPopBack() operation
    std::string result;
    // Read data from deque
    d->waitAndPopFront(result);
    // Write data into array
    array.push_back(result);
    // Stop if last entry was read (which should be 'vile.')
    if(result.compare("#") == 0){
      break;
    }
  }
}

void test::DequeTest::pushBackTestReaderThread(){
  // Array for storing the result
  std::vector<std::string> array;

  while(true){
    // String to store the result from the waitAndPopBack() operation
    std::string result;
    // Read data from deque
    d->waitAndPopBack(result);
    // Write data into array
    array.push_back(result);
    // Stop if last entry was read (which should be 'vile.')
    if(result.compare("white") == 0){
      break;
    }
  }
}
