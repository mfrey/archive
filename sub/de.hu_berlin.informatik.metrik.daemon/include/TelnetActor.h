#ifndef _TELNETACTOR_H__
#define _TELNETACTOR_H_

#include <string>
#include <fstream>
#include <iostream>

#include <vector>

#include "Telnet.h"
#include "Actor.h"

#include <unistd.h>
#include <termios.h>

#include "log4cxx/logger.h"

using namespace std;
using namespace de::hu_berlin::informatik::metrik::daemon;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          /**
           *
           */
          class TelnetActor : Actor {
            private: 
              /// A instance of the logger
              static log4cxx::LoggerPtr mLogger;
              /// The name of host (which will be connected)
              const char *mHostName;
              /// The port of the host (which will be connected)
              const char *mPort;
              /// The asynchronous IO service
              boost::asio::io_service mIOService;
              ///
              tcp::resolver::iterator mEndpointIterator;
              /// The structure hold the settings for the terminal interface
              termios mSettings;
              
              /// The method sets up the settings for the terminal interface
              void setup(void);

            public:
              ///
              TelnetActor(string, string);
              ///
              ~TelnetActor();
              ///
              void run(void);
              
          };
        }
      }
    }
  }
}

#endif
