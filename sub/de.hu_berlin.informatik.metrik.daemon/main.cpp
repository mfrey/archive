#include <iostream>

#include "include/notification.h"

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"


using namespace log4cxx;
using namespace log4cxx::helpers;

using namespace de::hu_berlin::informatik::metrik::daemon;

using namespace std;

int main(int argc, char **argv){
   Notification notification;
   notification.add("/vol/home-vol1/simulant/frey/tmp/test", IN_ACCESS);

   boost::thread workerThread(notification);

   workerThread.join();

   cout << "Hello World!\n" ;
   return 0;
}

