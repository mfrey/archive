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
    PropertyConfigurator::configure("logging.properties");
    Notification notification;

    LOG4CXX_INFO(logger, "Entering application.");
  }catch(Exception&){
    result = EXIT_FAILURE;
  }
  /*
  notification.add("/vol/home-vol1/simulant/frey/tmp/test", IN_ACCESS);

  boost::thread workerThread(notification);

  workerThread.join();
*/
  return result;
}

