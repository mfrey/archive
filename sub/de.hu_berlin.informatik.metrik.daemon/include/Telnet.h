#ifndef _TELNET_H_
#define _TELNET_H__

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
          class Telnet {
            private: 
              /// Size of the buffer
              static const int mBufferSize = 2048;
              ///
              boost::asio::io_service& mIOService;
              ///
              static log4cxx::LoggerPtr logger;
              /// The socket the telnet the instance is connected to
              tcp::socket mSocket; 
              ///
	      char read_msg_[mBufferSize]; 
              // data read from the socket
              deque<char> write_msgs_; // buffered write data

              ///
              void connect(tcp::resolver::iterator);
              ///
              void read(void);
              ///
	      void readComplete(const boost::system::error_code&, size_t);
              ///
              void connectComplete(const boost::system::error_code& pError, tcp::resolver::iterator pEndpointIterator);
              ///
              void writeToSocket(const char);
              ///
              void closeSocket(void);
              ///
              void writeStart(); 
              void writeComplete(const boost::system::error_code&);

            public:
              /// The constructor of the telnet class
              Telnet(boost::asio::io_service&, tcp::resolver::iterator);
              /// The destructor of the telnet class
              ~Telnet();
              /// The method initializes a telnet connection
              void open();
              /// The method writes data via an established telnet connection
              void write(const char);
              /// The method closes the telnet connection
              void close();
          };
        }
      }
    }
  }
}

#endif
