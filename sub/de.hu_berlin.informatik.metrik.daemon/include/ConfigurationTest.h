#ifndef _STUB_TEST_H_
#define _STUB_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Configuration.h"

#include "log4cxx/logger.h"

using namespace std;
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
            class ConfigurationTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(ConfigurationTest);
                CPPUNIT_TEST(testReadConfigurationFile);
                CPPUNIT_TEST(testGetLoggingFile);
                CPPUNIT_TEST(testGetConfigurationFile);
                CPPUNIT_TEST(testGetTelnetActorHost);
                CPPUNIT_TEST(testGetTelnetActorPort);
                CPPUNIT_TEST_SUITE_END();

                /// A logging instance
                static log4cxx::LoggerPtr mLogger;
                // Instances of the class which will be tested
                Configuration *c, *d;

              public:
                /// The set up method of the unit test
                void setUp(void);
                /// The tear down method of the unit test
                void tearDown(void);
                ///
                void testReadConfigurationFile();
                ///
                void testGetLoggingFile();
                ///
                void testGetConfigurationFile();
                ///
                void testGetTelnetActorHost();
                ///
                void testGetTelnetActorPort();
                ///
                void testGetFilesToWatch();
                ///
                void testGetDirectoriesToWatch();
                ///
                void parseOptions();
                ///
                void testSetConfigurationFile(void);
                ///
                void enableLogging(void);
                ///
                void testGetWatchDescriptorEntries();
                ///
                void testGetEntries();
            };
          }
        }
      }
    }
  }
}

#endif
