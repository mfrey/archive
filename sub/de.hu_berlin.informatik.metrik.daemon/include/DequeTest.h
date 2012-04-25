#ifndef _DEQUE_TEST_H_
#define _DEQUE_TEST_H_ 

#include <vector>

#include <unistd.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "Deque.h"

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
	namespace daemon {
          namespace test {
            class DequeTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(DequeTest);
                CPPUNIT_TEST(pushFrontTest);
//		CPPUNIT_TEST(pushFrontReaderWriterTest);
                CPPUNIT_TEST(pushBackTest);
//                CPPUNIT_TEST(pushBackReaderWriterTest);
                CPPUNIT_TEST_SUITE_END();

                /// A instance of the deque class 
                Deque<string> *d;
                /// A reader thread (which is required in order to test the concurrency abilities of the Deque class)
                boost::thread mReader;
                /// A writer thread (which is required in order to test the concurrency abilities of the Deque class)
                boost::thread mWriter;
                
                ///
                void pushFrontTestReaderThread(void);
                ///
                void pushBackTestReaderThread(void);
                ///
                void pushFrontTestWriterThread(void);
                ///
                void pushBackTestWriterThread(void);

              public:
                /// The set up method of the unit test
                void setUp(void);
                /// The tear down method of the unit test
                void tearDown(void);
                ///
                void pushFrontTest(void);
                ///
                void pushFrontReaderWriterTest(void);
                ///
                void pushBackTest(void);
                ///
                void pushBackReaderWriterTest(void);
                
            };
          }
        }
      }
    }
  }
}

#endif
