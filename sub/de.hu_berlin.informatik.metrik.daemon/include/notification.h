#ifndef _NOTIFICATION_H_ 
#define _NOTIFICATION_H_ 

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
            watchDescriptor *next;  
          };

          class Notification {
            private:
              static log4cxx::LoggerPtr logger;
              int mNotificationInstance;
              watchDescriptor *mList;
              watchDescriptor *mCurrent;

              void addEntry(int);
              void removeEntry();

            public:
              Notification();
              ~Notification();
              void add(const char*, uint32_t);
              void remove(const char*);
              void operator()();
          };
        }
      }
    }
  }
}

#endif
