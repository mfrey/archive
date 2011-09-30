#ifndef _NOTIFICATION_H_ 
#define _NOTIFICATION_H_ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sys/types.h"
#include "sys/inotify.h"

#include <iostream>
#include <vector>
#include <boost/thread.hpp>

#include "WatchDescriptorEntry.h"

#include "log4cxx/logger.h"

/** */
#define EVENT_SIZE        (sizeof (struct inotify_event))
/** */
#define EVENT_BUFFER_LEN  (1024 * ( EVENT_SIZE + 16 ))

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {

          struct watchDescriptor {
            // the watch descriptor
            int wd;
            // the length of the name field
            uint32_t length;
            // the name of the file/directory which will be watched (null terminated)
            char name[];
            // the mask of the watch descriptor 
            uint32_t mask;
            // a pointer to the previous entry
	    watchDescriptor *previous;  
            // a pointer to the next entry
            watchDescriptor *next;  
          };

          class Notification {
            private:
              static log4cxx::LoggerPtr logger;
              int mNotificationInstance;
              watchDescriptor *mFirst;
              watchDescriptor *mLast;
	      watchDescriptor *mCurrent;
              watchDescriptor *mPrevious;

              boost::thread mThread;

              void addEntry(int, uint32_t, const char*);
              void removeEntry();

            public:
              Notification();
              ~Notification();
              void add(const char*, uint32_t);
              void remove(const char*);
              void start();
              void run();
              void join();
              void addWatchDescriptorEntry(WatchDescriptorEntry);
              void addWatchDescriptorEntries(std::vector<WatchDescriptorEntry>);
          };
        }
      }
    }
  }
}

#endif
