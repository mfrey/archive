#include <iostream>

#include "include/notification.h"

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

using namespace de::hu_berlin::informatik::metrik::daemon;

using namespace std;

// TODO: Fixme
LoggerPtr logger(Logger::getLogger("cortex"));

int main(int argc, char **argv){
  int result = EXIT_SUCCESS;

  try {
    LOG4CXX_TRACE(logger, "starting application");
    // Read log4cxx property file
    PropertyConfigurator::configure("logging.properties");
    // Initialize notification object
    Notification notification;
    // Add directory to watch list
    notification.add("/vol/home-vol1/simulant/frey/tmp/test", IN_ACCESS);
    // Start worker thread
    //boost::thread notificationThread(notification);
    // Wait for thread to finish
    //notificationThread.join();

    LOG4CXX_TRACE(logger, "stopping application");
  }catch(Exception&){
    result = EXIT_FAILURE;
  }

  return result;
}

