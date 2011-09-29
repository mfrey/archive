#ifndef _NOTIFICATION_H_ 
#define _NOTIFICATION_H_ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sys/types.h"
#include "sys/inotify.h"

#include <iostream>
#include <boost/thread.hpp>

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
            int wd;
	    watchDescriptor *previous;  
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

              void addEntry(int);
              void removeEntry();

            public:
              Notification();
              ~Notification();
              void add(const char*, uint32_t);
              void remove(const char*);
              void start();
              void run();
              void join();
          };
        }
      }
    }
  }
}

#endif
