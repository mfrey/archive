#ifndef _DEQUE_TEST_H_
#define _DEQUE_TEST_H_ 

#include <vector>

#include <unistd.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "Deque.h"

#include "log4cxx/logger.h"

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
                /// A instance of the logger
                static log4cxx::LoggerPtr mLogger;

                CPPUNIT_TEST_SUITE(DequeTest);
                CPPUNIT_TEST(pushFrontTest);
		CPPUNIT_TEST(pushFrontReaderWriterTest);
                CPPUNIT_TEST(pushBackTest);
                CPPUNIT_TEST(pushBackReaderWriterTest);
                CPPUNIT_TEST_SUITE_END();

                /// A instance of the deque class 
                Deque<string> *d;
                /// A reader thread (which is required in order to test the concurrency abilities of the Deque class)
                boost::thread mReader;
                /// A writer thread (which is required in order to test the concurrency abilities of the Deque class)
                boost::thread mWriter;
                
                ///
                void popFrontThread(void);
                ///
                void popBackThread(void);
                ///
                void pushFrontThread(void);
                ///
                void pushBackThread(void);
                /// 
                std::vector<std::string> splitString(std::string);

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
