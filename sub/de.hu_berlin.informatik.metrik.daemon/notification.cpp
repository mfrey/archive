#include "include/notification.h"


Notification::Notification(){
  this->mNotificationInstance = inotify_init();
  // An error occured whil initializing inotify
  if(this->mNotificationInstance < 0){

  }
}

Notification::~Notification(){
  // Only close the descriptor if opening it was successful
  if(this->mNotificationInstance >= 0){
    close(this->mNotificationInstance);
  }
}

void Notification::addDirectory(){

}

void Notification::addFile(){

}

void Notification::addDirectories(){

}

void Notification::addFiles(){

}

void Notification::removeDirectory(){

}

void Notification::removeFile(){

}

void Notification::removeDirectories(){

}

void Notification::removeFiles(){

}

