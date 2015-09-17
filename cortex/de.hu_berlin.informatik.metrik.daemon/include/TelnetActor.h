#ifndef _TELNETACTOR_H__
#define _TELNETACTOR_H_

#include <string>
#include <fstream>
#include <iostream>

#include <vector>

#include "Actor.h"
#include "Telnet.h"
#include "Deque.h"

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
              /// The asynchronous IO service
              boost::asio::io_service mIOService;
              ///
              tcp::resolver::iterator mEndpointIterator;
              /// A instance of the logger
              static log4cxx::LoggerPtr mLogger;
              /// The name of host (which will be connected)
              std::string mHostName;
              /// The port of the host (which will be connected)
              std::string mPort;
              /// The structure hold the settings for the terminal interface
              termios mSettings;
              /// The write buffer
              Deque<string> mWriteBuffer;
              /// The read buffer
              Deque<string> mReadBuffer;
              /// The method sets up the settings for the terminal interface
              void setup(void);

            public:
              /// The constructor of the class
              TelnetActor(string, string);
              /// The destructor of the class
              ~TelnetActor();
              /// A method which runs a thread (which in turn reads/write data via telnet)
              void run(void);
              /// A method to send data (via telnet)
              void send(string);
              /// A method to receive data (via telnet)
              string recv(void);
          };
        }
      }
    }
  }
}

#endif
