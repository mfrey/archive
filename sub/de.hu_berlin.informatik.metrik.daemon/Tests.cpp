#include "include/WatchDescriptorEntry.h"
#include "include/WatchDescriptorEntryTest.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>

int main(int argc, char **argv){
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  CppUnit::TextTestRunner runner;

  runner.addTest( suite );

  runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
                                                       std::cerr ) );
  bool wasSucessful = runner.run();

  return wasSucessful ? 0 : 1;
}
