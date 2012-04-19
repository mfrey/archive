#include "include/Notification.h"

using namespace de::hu_berlin::informatik::metrik::daemon;
using namespace log4cxx;

LoggerPtr Notification::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.notification"));

Notification::Notification(){
  // An error occured while initializing inotify
  if((this->mNotificationInstance = inotify_init()) < 0){
    LOG4CXX_FATAL(logger, "initializing notification instance (inotify_init) was not successful");
    // TODO: Error Handling
  }
}

Notification::~Notification(){
  // Close the watch descriptors stored in the watch descriptor entry list
  this->closeWatchDescriptors();
  // Erase the watch descriptor list
  this->eraseWatchDescriptorList();
  // Close the file handle for the inotify instance
  this->closeInotifyInstance();
}

bool Notification::add(const char *pPathName, uint32_t pMask){
  LOG4CXX_TRACE(logger, "add file/directory: " << pPathName << " to list");
  if(this->mNotificationInstance >= 0){
    int wd = -1;
    // Add files or directories to the watch list
    if((wd = inotify_add_watch(this->mNotificationInstance, pPathName, pMask)) != 0){
      LOG4CXX_TRACE(logger, "trying to add watch descriptor " << wd << " to list");
      WatchDescriptorEntry entry(wd, pMask, pPathName);
      this->mList[pPathName] = entry;
      return true;
    }
  }
  return false;
}

int Notification::getInotifyId(){
  return this->mNotificationInstance;
}
/**
 *
 */
bool Notification::remove(const char* pPathName){
  if(this->mNotificationInstance >= 0){
    WatchDescriptorEntry entry = this->mList[pPathName];

    /**
     *  If there is no entry for the given path name, the unordered map will create 
     *  a new entry using the default constructor of the class. This leads to the
     *  ridiculous check if the value of watch descriptor is -1 (which should never
     *  be the case).
     */
    if(entry.getId() != -1){
      LOG4CXX_TRACE(logger, "trying to remove watch descriptor " << entry.getId());

      if(inotify_rm_watch(this->mNotificationInstance, entry.getId()) != 0){
        // TODO: Fehlerbehandlung

      }else{
        int id = entry.getId();
        this->mList.erase(pPathName);
        LOG4CXX_TRACE(logger, "removed watch descriptor " << id);
        return true;
      }
    }else{
      LOG4CXX_FATAL(logger, "removing watch descriptor for file/directory " <<  pPathName << "  was not successful");
      // TODO: Fehlerbehandlung
    }
  }

  return false;
}

bool Notification::addWatchDescriptorEntry(WatchDescriptorEntry pEntry){
  LOG4CXX_TRACE(logger, "add entry with watch descriptor " << pEntry.getId() << " mask " << pEntry.getMask() << " file/directory " << pEntry.getName() << " to list");
  // TODO: This doesn't feel good
  std::pair<map::iterator, bool> result;
  result = mList.insert(std::pair<const char*,WatchDescriptorEntry>(pEntry.getName().c_str(), pEntry));
  return result.second;
}

bool Notification::addWatchDescriptorEntries(map pEntries){
  // Current size of the hash map
  unsigned int previousSize = mList.size();
  // New size of the hash map (after successful addition of the new entries)
  unsigned int expectedNewSize = previousSize + pEntries.size();
  // Add new elements to the list
  mList.insert(pEntries.begin(), pEntries.end());
  // Compare new size versus expected size
  return (mList.size() == expectedNewSize);
}

void Notification::addEntry(int pWatchDescriptor, uint32_t pMask, const char *pPathName){
  LOG4CXX_TRACE(logger, "create entry with watch descriptor " << pWatchDescriptor << " mask " << pMask << " file/directory " << pPathName << " to list");
  WatchDescriptorEntry entry(pWatchDescriptor, pMask, pPathName);
  this->addWatchDescriptorEntry(entry);
}

/**
 *
 */
void Notification::run(){
  LOG4CXX_TRACE(logger, "starting notification thread");
  // Iterator variable for iterating over the result array
  int i, length = 0;
  // The result buffer
  char buffer[EVENT_BUFFER_LEN];
  LOG4CXX_TRACE(logger, "set read buffer to " << EVENT_BUFFER_LEN);
  // Read permanently 
  // TODO: Built 'break' criteria
  while((length = read(this->mNotificationInstance, buffer, EVENT_BUFFER_LEN)) >= 0){
    // Iterate over the results
    while(i < length){
      // Get event from buffer
      struct inotify_event *event = (struct inotify_event *) &buffer[i]; 
      //
      if(event->mask & IN_ACCESS){
        LOG4CXX_TRACE(logger, "file was accessed");
      }
/*
      }else if(event->mask & ){

      }else if(event->mask & ){

      }else if(event->mask & ){

      }else if(event->mask & ){

      }
*/
      // Move the position to the next event
      i += EVENT_SIZE + event->len;
    }
    // Reset the iterator
    i = 0;
  }
  LOG4CXX_TRACE(logger, "stopping notification thread");
}

void Notification::start(){
  LOG4CXX_TRACE(logger, "initialize notification thread");
  this->mThread = boost::thread(&Notification::run, this); 
}

void Notification::join(){
  this->mThread.join();
}

void Notification::closeWatchDescriptors(){
  for(map::iterator i = this->mList.begin(); i != this->mList.end(); i++){
     if(inotify_rm_watch(this->mNotificationInstance, i->second.getId()) != 0){
       LOG4CXX_FATAL(logger, "inotify_rm_watch() on watch descriptor " << i->second.getId() << " for file/directory " << i->second.getName() << " failed");
     }else{
       LOG4CXX_TRACE(logger, "inotify_rm_watch() on watch descriptor " << i->second.getId() << " for file/directory " << i->second.getName() << " was successful");
     }
  }
}

void Notification::eraseWatchDescriptorList(){
  // TODO: Does it also erase the last element?
  this->mList.erase(this->mList.begin(), this->mList.end());
}

void Notification::closeInotifyInstance(){
  if(close(this->mNotificationInstance) != 0){
    // TODO: Error handling
    LOG4CXX_FATAL(logger, "close() on notification instance file descriptor failed");
  }
}
