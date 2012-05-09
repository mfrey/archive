#ifndef _SOCKET_H_ 
#define _SOCKET_H_

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


#include <string>
#include <string.h>

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Socket {
            private: 
              ///
              std::string mHostName;
              ///
              std::string mPort;
              ///
              int mSocket;
              ///
              struct sockaddr_storage their_addr;
              ///
              struct addrinfo mHints;
              ///
              socklen_t mAddressSize;

              void *getSocketAddress(struct sockaddr*);
              void error(const char*);

              static const int mBufferSize = 512;

              unsigned char mBuffer[mBufferSize];

            public:
              Socket();
              Socket(std::string, std::string);
              ~Socket();
              int open();
              void closeSocket();
              void run();
              void setup();
          };
        }
      }
    }
  }
}

#endif
