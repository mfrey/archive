#ifndef _SOCKET_TEST_H_
#define _SOCKET_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Socket.h"

#include "log4cxx/logger.h"

using namespace de::hu_berlin::informatik::metrik::daemon;

/**
 * THIS IS A STUB: PLEASE FILL ME WITH LIFE.
 */
namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
	namespace daemon {
          namespace test {
            class SocketTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(SocketTest);
                CPPUNIT_TEST(testOpen);
                CPPUNIT_TEST_SUITE_END();

                /// A logger instance 
                static log4cxx::LoggerPtr mLogger;

                // Instances of the class which will be tested
                Socket *s;

              public:
                /// The set up method of the unit test
                void setUp(void);
                /// The tear down method of the unit test
                void tearDown(void);

                void testOpen();
            };
          }
        }
      }
    }
  }
}

#endif
