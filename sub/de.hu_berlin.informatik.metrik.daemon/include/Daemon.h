#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "log4cxx/logger.h"
#include "Notification.h"
#include "configuration.h"

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Daemon {
            private: 
              Configuration mConfiguration;
              Notification mNotification;

            public:
              Daemon();
              Daemon(Configuration);
              ~Daemon();
              void start();
              void stop();
          };
        }
      }
    }
  }
}

#endif 
