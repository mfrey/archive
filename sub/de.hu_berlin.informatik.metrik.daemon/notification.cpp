#include "include/notification.h"


Notification::Notification(int pSize){
  // Set the list to NULL
  this->mList = NULL;

  // An error occured while initializing inotify
  if((this->mNotificationInstance = inotify_init()) == 0){
    // Create a first element of the list
    this->mList = (watchDescriptor*) malloc(sizeof(watchDescriptor));
    // Set the head of the list to the first element
    this->mHead = this->mList;
    
    if(this->mList != NULL){
      for(int i = 0; i <  (pSize-1); i++){
         // Create a new element
         watchDescriptor *entry = (watchDescriptor*) malloc(sizeof(watchDescriptor));
         // Check if allocation was successful
         if(entry != NULL){
           // Add entry to list
           this->mList->next = entry;
           // Update the current head of the list
           this->mHead = entry;
         } 
      }
    // Allocation failed
    }else{
      // TODO: Error Handling
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
         watchDescriptor *current = this->mList;
         // Get the 'next' element
         if(current->next != NULL){
           watchDescriptor *next = current->next;
           // Set the element after the current element to the new head of the list
           this->mList = next;
           // Reset the pointer of the old entry
           current->next = NULL;
         }
         // Free the memory
         free(current); 
      }
      // Reset the head
      this->mHead = NULL;
      // Free the last element
      free(this->mList);
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
      watchDescriptor *entry = (watchDescriptor*) malloc(sizeof(watchDescriptor));
      entry->wd = wd;
      entry->next = NULL;
      // Add the element to the end of the list
      this->mHead->next = entry; 
      // Move head pointer to the last entry
      this->mHead = entry;
    }
  }
}

void Notification::remove(){
  if(this->mNotificationInstance >= 0){

  }
}


