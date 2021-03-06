#ifndef _NOTIFICATION_TEST_H_
#define _NOTIFICATION_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Notification.h" 

#include <stdlib.h>
#include <fstream>
#include <string>
#include <time.h>
#include <sstream>

#include "log4cxx/logger.h"

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
	namespace daemon {
          namespace test {
            /**
             * The class provides a unit test for the class Notification.
             */
            class NotificationTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(NotificationTest);
                CPPUNIT_TEST(testAddRemove);
                CPPUNIT_TEST(testAddRemoveWatchDescriptorEntry);
                CPPUNIT_TEST(testAddRemoveWatchDescriptorEntries);
                CPPUNIT_TEST_SUITE_END();
 
                /// A logging instance
                static log4cxx::LoggerPtr mLogger;
                // Instances of the class which will be tested
                Notification *a, *b;
                /// The helper method allows to create a single watch descriptor entry
                WatchDescriptorEntry createWatchDescriptorEntry(const char*);
                /// The helper method allows to create a file
                void createFile(const char*);
                /// The helper method allows to create a file name
                string createFileName(void);

              public:
                /// The set up method of the unit test
                void setUp(void);
                /// The tear down method of the unit test
                void tearDown(void);
                /// The method tests the add/remove method of class Notification
                void testAddRemove();
                /// The method tests the add/remove method for an WatchDescriptorEntry object in class Notification
                void testAddRemoveWatchDescriptorEntry();
                /// The method tests the add/remove method for an list of WatchDescriptorEntry objects in class Notification
                void testAddRemoveWatchDescriptorEntries();
            };
          }
        }
      }
    }
  }
}

#endif
