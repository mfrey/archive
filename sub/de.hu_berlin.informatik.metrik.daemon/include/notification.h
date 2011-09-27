#ifndef _NOTIFICATION_H_ 
#define _NOTIFICATION_H_ 

#include <unistd.h>
#include <stdlib.h>
#include "sys/types.h"
#include "sys/inotify.h"

#include <iostream>
#include <boost/thread.hpp>

/** */
#define EVENT_SIZE        (sizeof (struct inotify_event))
/** */
#define EVENT_BUFFER_LEN  (1024 * ( EVENT_SIZE + 16 ))


struct watchDescriptor {
  int wd;
  watchDescriptor *next;  
};

class Notification {
  private:
    int mNotificationInstance;
    watchDescriptor *mList;
    watchDescriptor *mHead;
    watchDescriptor *mFree;

    void addEntry(int);
    void removeEntry();

  public:
    Notification();
    ~Notification();
    void add(const char*, uint32_t);
    void remove(const char*);
    void operator()();
};

#endif
