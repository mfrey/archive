#ifndef _ACTORTELNET_H_
#define _ACTORTELNET_H__

#include "Actor.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "log4cxx/logger.h"

using namespace std;
using boost::asio::ip::tcp;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class ActorTelnet : Actor {
            private: 
              ///
              boost::asio::io_service mIOService;
              ///
              static log4cxx::LoggerPtr logger;
              /// The socket the telnet the instance is connected to
              tcp::socket mSocket; 
              ///
	      char read_msg_[max_read_length]; 
              // data read from the socket
              deque<char> write_msgs_; // buffered write data
              ///
              boost::system::error_code mError;

              ///
              void connect();
              ///
              void writeToSocket(const char);
              ///
              void closeSocket(void);

            public:
	      ActorTelnet(struct sockaddr_in, struct hostent*);
              ActorTelnet();
              void open();
              void write(const char*);
              void close();
              ~ActorTelnet();
          };
        }
      }
    }
  }
}

#endif
