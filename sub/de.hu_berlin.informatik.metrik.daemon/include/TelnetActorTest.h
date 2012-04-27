#ifndef _TELNET_ACTOR_TEST_H_
#define _TELNET_ACTOR_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "log4cxx/logger.h"

#include "TelnetActor.h"

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
	namespace daemon {
          namespace test {
            /**
             * 
             */
            class TelnetActorTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(TelnetActorTest);
                CPPUNIT_TEST(testRun);
                CPPUNIT_TEST_SUITE_END();
                /// A logger instance 
                static log4cxx::LoggerPtr mLogger;
                boost::thread mActor;
                ///
                boost::thread mWriter;

                // Instance of the class which will be tested
                TelnetActor *t;
                /// A method which will be run by a thread which in turn will write data to the telnet actor  
		void writeDataThread(void);
                ///
                void telnetActorThread(void);

              public:
                /// The set up method of the unit test
                void setUp(void);
                /// The tear down method of the unit test
                void tearDown(void);
                /// The method tests the run() method of class TelnetActor
                void testRun(void);
            };
          }
        }
      }
    }
  }
}

#endif
