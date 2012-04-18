#include "include/Daemon.h"

using namespace de::hu_berlin::informatik::metrik::daemon;
using namespace log4cxx;

LoggerPtr Daemon::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.daemon"));

Daemon::Daemon(){

}

Daemon::Daemon(Configuration pConfiguration){
  this->mConfiguration = pConfiguration; 
}

Daemon::~Daemon(){

}

void Daemon::start(){
  //std::vector<WatchDescriptorEntry> list = this->mConfiguration.getWatchDescriptorEntries();
  // Add watch directory entries from configuration file
  //this->mNotification.addWatchDescriptorEntries(list);
  // Initialize the notification thread 
  //this->mNotification.start();

}

void Daemon::stop(){
  
}
