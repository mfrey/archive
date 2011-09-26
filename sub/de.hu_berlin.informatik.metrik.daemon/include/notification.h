#ifndef _NOTIFICATION_H_ 
#define _NOTIFICATION_H_ 

#include <unistd.h>
#include "sys/inotify.h"

class Notification {
  private:
    int mNotificationInstance;

  public:
    Notification();
    ~Notification();
    void addDirectory();
    void removeDirectory();
    void addDirectories();
    void removeDirectories();
    void addFile();
    void addFiles();
    void removeFiles();
    void removeFile();
};

#endif
