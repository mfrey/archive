#include "include/notification.h"

using namespace de::hu_berlin::informatik::metrik::daemon;
using namespace log4cxx;

LoggerPtr Notification::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.notification"));

Notification::Notification(){
  // Set the list to NULL
  this->mList = NULL;

  // An error occured while initializing inotify
  if((this->mNotificationInstance = inotify_init()) >= 0){
    LOG4CXX_TRACE(logger, "Initializing notification instance (inotify_init) was successful");
    // Create a first element of the list
    this->mList = (watchDescriptor*) malloc(sizeof(watchDescriptor));
    LOG4CXX_TRACE(logger, "allocating watch descriptor structure");
    if(this->mList != NULL){
      LOG4CXX_TRACE(logger, "allocating watch descriptor structure was successful");
      this->mList->wd = -1;
      this->mList->next = NULL;
      this->mCurrent = this->mList;
    }else{
      // TODO
      LOG4CXX_FATAL(logger, "malloc() failed on allocating memory for watch descriptor structure");
    }
  // Initializiation failed
  }else{
    LOG4CXX_FATAL(logger, "initializing notification instance (inotify_init) was not successful");
    // TODO: Error Handling
  }
}

Notification::~Notification(){
  // Only close the descriptor if opening it was successful
  if(this->mNotificationInstance != 0){
    watchDescriptor *temp = this->mList;

    // Only if the list has been allocated
    if(this->mList != NULL){
      // Iterate over the list
      while(temp != NULL){
	 this->mList= temp->next;
/*
         // Close the watch descriptor
         if(inotify_rm_watch(this->mNotificationInstance, this->mFirst->wd) != 0){
           // Error handling
           LOG4CXX_FATAL(logger, "inotify_rm_watch() on watch descriptor failed");
         } 
*/
         free(temp);
         temp = this->mList;
      }

      free(temp);
      // Reset the last pointer to the list
      this->mCurrent= NULL;
      // Reset the list
      this->mList = NULL;
    }
    
    if(close(this->mNotificationInstance) != 0){
      // Error handling
      LOG4CXX_FATAL(logger, "close() on notification instance file descriptor failed");
    }
  }
}

void Notification::add(const char* pPathName, uint32_t pMask){
  if(this->mNotificationInstance >= 0){
    int wd = -1;
    // Add files or directories to the watch list
    if((wd = inotify_add_watch(this->mNotificationInstance, pPathName, pMask)) != 0){
      this->addEntry(wd);
    }
  }
}

void Notification::remove(const char* pPathName){
  if(this->mNotificationInstance >= 0){
/*
    if(inotify_rm_watch(this->mNotificationInstance, ) != 0){
      // Error handling
    } 
*/
  }
}

void Notification::addEntry(int pWatchDescriptor){
  //
  if(this->mList->wd != -1){
    watchDescriptor *entry = (watchDescriptor*) malloc(sizeof(watchDescriptor));
    if(entry != NULL){
      entry->wd = pWatchDescriptor;
      entry->next = NULL;
      this->mCurrent->next = entry;
      this->mCurrent = entry;
    }else{
      // TODO
    }
  }else{
    this->mList->wd = pWatchDescriptor;
  }
}

/**
 *
 */
void Notification::operator()(){
  // Iterator variable for iterating over the result array
  int i, length = 0;
  // The result buffer
  char buffer[EVENT_BUFFER_LEN];
  // Read permanently 
  // TODO: Built 'break' criteria
  while((length = read(this->mNotificationInstance, buffer, EVENT_BUFFER_LEN)) >= 0){
    // Iterate over the results
    while(i < length){
      // Get event from buffer
      struct inotify_event *event = (struct inotify_event *) &buffer[i]; 
      //
      if(event->mask & IN_ACCESS){
       std::cout << "bla" << std::endl; 
      }
      // Move the position to the next event
      i += EVENT_SIZE + event->len;
    }
    // Reset the iterator
    i = 0;
  }
}

