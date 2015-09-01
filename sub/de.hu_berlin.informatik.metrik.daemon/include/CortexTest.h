#ifndef _CORTEX_TEST_H_
#define _CORTEX_TEST_H_ 

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

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
            class CortexTest : public CppUnit::TestFixture {
              private:
                CPPUNIT_TEST_SUITE(CortexTest);
                // CPPUNIT_TEST(testAddRemove);
                CPPUNIT_TEST_SUITE_END();

                // Instances of the class which will be tested
                //Notification *a, *b;

              public:
                /// The set up method of the unit test
                void setUp(void);
                /// The tear down method of the unit test
                void tearDown(void);
            };
          }
        }
      }
    }
  }
}

#endif
