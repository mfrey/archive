#ifndef _WATCH_DESCRIPTOR_ENTRY_TEST_H_
#define _WATCH_DESCRIPTOR_ENTRY_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "log4cxx/logger.h"
#include "WatchDescriptorEntry.h" 

using namespace de::hu_berlin::informatik::metrik::daemon;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
	namespace daemon {
          namespace test {
            class WatchDescriptorEntryTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(WatchDescriptorEntryTest);
                CPPUNIT_TEST(testSetId);
                CPPUNIT_TEST(testGetId);
                CPPUNIT_TEST(testSetName);
                CPPUNIT_TEST(testGetName);
                CPPUNIT_TEST(testSetMask);
                CPPUNIT_TEST(testGetMask);
                CPPUNIT_TEST_SUITE_END();

                WatchDescriptorEntry *a, *b;
                /// A logger instance 
                static log4cxx::LoggerPtr logger;
                /// The id of the watchdescriptor entry
                static const int mId;
                /// The mask of the file which will be watched
                static const uint32_t mMask;
                /// The name of the file
                static const std::string mName;

              public:
                //
                void setUp(void);
                //
                void tearDown(void);
                //
                void testSetId();
                //
                void testGetId();
                //
                void testSetName();
                //
                void testGetName();
                //
                void testSetMask();
                //
                void testGetMask();
            };
          }
        }
      }
    }
  }
}

#endif
