#include <iostream>

#include "include/TelnetActor.h"
#include "include/Notification.h"
#include "include/Configuration.h"

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

void usage(){
  cout << "Usage: cortex <arguments>" << endl;
  cout << "   --help            -h   Display this help screen" << endl;
  cout << "   --config=arg      -c   Set the config file path" << endl;
  cout << "   --log=arg         -l   Set the logging file path" << endl;
}

int main(int argc, char **argv){
  int result = EXIT_SUCCESS;

    // Read the configuration file
    Configuration configuration(string("settings/default.conf"));
    // Initialize notification thread 
    Notification notification(configuration.getEntries());
    // Intialize (telnet) actor
    TelnetActor actor(configuration.getTelnetActorHost(), configuration.getTelnetActorPort());
/*
    // Add directory to watch list
    notification.add("/proc", IN_ACCESS);
*/
    // Start thread
    notification.start();
    // Wait till thread finishes
    notification.join();

  return result;
}

