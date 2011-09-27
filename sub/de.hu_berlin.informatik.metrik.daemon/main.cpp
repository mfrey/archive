#include <iostream>

#include "include/notification.h"

using namespace std;

int main(int argc, char **argv){
   Notification notification;
   notification.add("/vol/home-vol1/simulant/frey/tmp/test", IN_ACCESS);

   boost::thread workerThread(notification);

   cout << "Hello World!\n" ;
   return 0;
}

