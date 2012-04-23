#include "include/WatchDescriptorEntry.h"
#include "include/WatchDescriptorEntryTest.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr logger(Logger::getLogger("cortex unit test"));

int main(int argc, char **argv){

  try {
    // Read log4cxx property file for unit tests
    PropertyConfigurator::configure("logging_cppunit.properties");
    LOG4CXX_TRACE(logger, "starting unit test");
    //
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(suite);
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));
    bool wasSucessful = runner.run();

    LOG4CXX_TRACE(logger, "stopping unit test");

    return wasSucessful ? 0 : 1;
  }catch(Exception& e){
    std::cerr << "An exception has occurred " << e.what() << std::endl;
    return -1;
  }
}
