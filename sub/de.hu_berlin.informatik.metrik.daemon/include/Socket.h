#ifndef _SOCKET_H_ 
#define _SOCKET_H_

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
// TODO: That's not so cool. Fix! Added for memset
#include <string.h>

#include "log4cxx/logger.h"

// Header for: perror()
#include <stdio.h>
#include <errno.h>

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Socket {
            private: 
	      /// An instance of the log4cxx logger
	      static log4cxx::LoggerPtr mLogger;

              /// The name of the host which should be connected
              std::string mHostName;
              /// The port of the host which should be connected
              std::string mPort; 
              /// A file descriptor for the socket
              int mSocket;
              ///
              struct sockaddr_storage their_addr;
              ///
              struct addrinfo mHints;
              ///
              socklen_t mAddressSize;

              /// The method returns the address of an socket (IPv4 or IPv6)
              void *getSocketAddress(struct sockaddr*);
              /**
               * A method which prints an error message (given as a parameter) and 
               * the corresponding errno message, if errno was set.
               */
              void error(const char*);

              /// The size of the receive buffer
              static const int mBufferSize = 512;
              /// The receive buffer
              unsigned char mBuffer[mBufferSize];

            public:
              Socket();
              /// The constructor sets up communication
              Socket(std::string, std::string);
              ~Socket();
              /// The method opens a socket
              int open();
              void closeSocket();
              void run();
              void setup();

              /// The method writes a string to the socket
              void write(std::string);

              std::string getHostName();
              void setHostName(std::string);

              std::string getPort();
              void setPort(std::string);
          };
        }
      }
    }
  }
}

#endif
