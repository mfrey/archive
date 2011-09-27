#include "include/notification.h"


Notification::Notification(){
  // Set the list to NULL
  this->mList = NULL;

  // An error occured while initializing inotify
  if((this->mNotificationInstance = inotify_init()) >= 0){
    // Create a first element of the list
    this->mList = (watchDescriptor*) malloc(sizeof(watchDescriptor));

    if(this->mList != NULL){
      // Set to be uninitialized
      this->mList->wd = -1;
      // Set the head of the list to the first element
      this->mHead = this->mList;
    }else{
      // TODO
    }
  // Initializiation failed
  }else{
    // TODO: Error Handling
  }
}

Notification::~Notification(){
  // Only close the descriptor if opening it was successful
  if(this->mNotificationInstance != 0){
    // Only if the list has been allocated
    if(this->mList != NULL){
      // Iterate over the list
      while(this->mList->next != NULL){
         // Close the watch descriptor
         if(inotify_rm_watch(this->mNotificationInstance, this->mList->wd) != 0){
           // Error handling
         } 
         // Get the 'current' element
         watchDescriptor *first = this->mList;
         // Get the 'next' element
         if(first->next != NULL){
           watchDescriptor *next = first->next;
           // Set the element after the current element to the new head of the list
           this->mList = next;
           // Reset the pointer of the old entry
           first->next = NULL;
         }
         // Free the memory
         free(first); 
      }
      // Reset the head
      this->mHead = NULL;
      // Reset the pointer to free entry
      this->mFree = NULL;
    }
    
    if(close(this->mNotificationInstance) != 0){
      // Error handling
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
      this->mHead->next = entry;
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
