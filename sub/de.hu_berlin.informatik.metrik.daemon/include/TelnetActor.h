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
              /// The name of host (which will be connected)
              char *mHostName;
              /// The port of the host (which will be connected)
              char *mPort;
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
              TelnetActor(char*, char*);
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
