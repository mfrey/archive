#ifndef _NOTIFICATION_H_ 
#define _NOTIFICATION_H_ 

#include <unistd.h>
#include <stdlib.h>
#include "sys/inotify.h"

struct watchDescriptor {
  int wd;
  watchDescriptor *next;  
};

class Notification {
  private:
    int mNotificationInstance;
    watchDescriptor *mList;
    watchDescriptor *mHead;

  public:
    Notification(int);
    ~Notification();
    void add(const char*, uint32_t);
    void remove();
};

#endif
