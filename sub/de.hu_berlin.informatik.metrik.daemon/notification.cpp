#include "include/notification.h"

using namespace de::hu_berlin::informatik::metrik::daemon;
using namespace log4cxx;

LoggerPtr Notification::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.notification"));

Notification::Notification(){
  // Set the list pointers to null
  this->mFirst = this->mLast = this->mCurrent = this->mPrevious = NULL;

  // An error occured while initializing inotify
  if((this->mNotificationInstance = inotify_init()) < 0){
    LOG4CXX_FATAL(logger, "initializing notification instance (inotify_init) was not successful");
    // TODO: Error Handling
  }
}

Notification::~Notification(){
  watchDescriptor* temp = this->mFirst;
  if(this->mFirst != NULL){
    while(temp->next != NULL){
      if(inotify_rm_watch(this->mNotificationInstance, temp->wd) != 0){
        // Error handling
        LOG4CXX_FATAL(logger, "inotify_rm_watch() on watch descriptor " << temp->wd << " failed");
      }else{
        LOG4CXX_TRACE(logger, "removed watch descriptor " << temp->wd << " from list");
      }
      LOG4CXX_TRACE(logger, "attempt to remove watch descriptor " << temp->wd << " from list");
      this->mFirst = temp->next;
      free(temp);
      LOG4CXX_TRACE(logger, "sucessfully removed watch descriptor from list");
      temp = this->mFirst;
    }

    if(inotify_rm_watch(this->mNotificationInstance, temp->wd) != 0){
      // Error handling
      LOG4CXX_FATAL(logger, "inotify_rm_watch() on watch descriptor " << temp->wd << " failed");
    }else{
      LOG4CXX_FATAL(logger, "inotify_rm_watch() on watch descriptor " << temp->wd << " was successful");
    }

    LOG4CXX_TRACE(logger, "attempt to remove watch descriptor " << temp->wd << " from list");
  
    free(temp);

    LOG4CXX_TRACE(logger, "sucessfully removed watch descriptor from list");

    this->mFirst = NULL;
    this->mCurrent = NULL;
  }
    
  if(close(this->mNotificationInstance) != 0){
    // Error handling
    LOG4CXX_FATAL(logger, "close() on notification instance file descriptor failed");
  }
}

void Notification::add(const char* pPathName, uint32_t pMask){
  LOG4CXX_TRACE(logger, "add file/directory: " << pPathName << " to list");
  if(this->mNotificationInstance >= 0){
    int wd = -1;
    // Add files or directories to the watch list
    if((wd = inotify_add_watch(this->mNotificationInstance, pPathName, pMask)) != 0){
      LOG4CXX_TRACE(logger, "trying to add watch descriptor " << wd << " to list");
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
  watchDescriptor* newDescriptor = NULL;

  if(this->mFirst == NULL){
    this->mFirst = (watchDescriptor*) malloc(sizeof(watchDescriptor));

    if(this->mFirst == NULL){
      LOG4CXX_FATAL(logger, "malloc() on first entry of watch descriptor list failed");
      // TODO
    }else{
      LOG4CXX_TRACE(logger, "adding watch descriptor " << pWatchDescriptor << " to list");
      this->mFirst->wd = pWatchDescriptor;
      this->mFirst->next   = NULL;
      this->mFirst->previous   = NULL;
      this->mCurrent = this->mFirst;
      this->mLast = this->mFirst;
    }
  }else{
    newDescriptor = (watchDescriptor*) malloc(sizeof(watchDescriptor));

    if(newDescriptor == NULL){
      LOG4CXX_FATAL(logger, "malloc() on new entry of watch descriptor list failed");
      // TODO
    }else{
      LOG4CXX_TRACE(logger, "adding watch descriptor " << pWatchDescriptor << " to list");
      newDescriptor->wd = pWatchDescriptor;
      newDescriptor->next   = NULL;
      newDescriptor->previous   = this->mLast;
      this->mLast->next = newDescriptor;
      this->mLast = newDescriptor;
    }
  }
}

/**
 *
 */
void Notification::operator()(){ 
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
      LOG4CXX_TRACE(logger, "read event from");
      //
      if(event->mask & IN_ACCESS){
        LOG4CXX_TRACE(logger, "file was accessed");
      }
      // Move the position to the next event
      i += EVENT_SIZE + event->len;
    }
    // Reset the iterator
    i = 0;
  }
  LOG4CXX_TRACE(logger, "stopping notification thread");
}

