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
#include <boost/tr1/unordered_map.hpp>
#include <utility>

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
          class Notification {
            private:
              static log4cxx::LoggerPtr logger;
              int mNotificationInstance;
              
              typedef std::tr1::unordered_map<const char*, WatchDescriptorEntry> map;
              map mList;

              boost::thread mThread;

              void addEntry(int, uint32_t, const char*);
              void removeEntry();
              void closeWatchDescriptors();
              void eraseWatchDescriptorList();
              void closeInotifyInstance();
              void initializeInotify();

            public:
              Notification();
              Notification(std::vector<std::string>);
              ~Notification();
              int getInotifyId();
              bool add(const char*, uint32_t);
              bool remove(const char*);
              void start();
              void run();
              void join();
              void createWatchDescriptorEntries(std::vector<std::string>);
              bool addWatchDescriptorEntry(WatchDescriptorEntry);
              bool addWatchDescriptorEntries(map);
          };
        }
      }
    }
  }
}

#endif
