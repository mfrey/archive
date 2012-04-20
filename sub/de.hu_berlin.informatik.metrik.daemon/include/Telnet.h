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
              /// A 'handler' for asynchronous IO
              boost::asio::io_service& mIOService;
              /// An instance of the log4cxx logger
              static log4cxx::LoggerPtr logger;
              /// The socket the telnet the instance is connected to
              tcp::socket mSocket; 
              /// A buffer for writing data
	      char mBuffer[mBufferSize]; 
              // A deque for reading data
              deque<char> write_msgs_; 

              /// The method begins to opens a telnet connection
              void connect(tcp::resolver::iterator);
              /// The method finishes to open a telnet connection, indepdent of success/failure
              void connectComplete(const boost::system::error_code& pError, tcp::resolver::iterator pEndpointIterator);
              /// The method begins to read data from an established telnet connection
              void readStart(void);
              /// The method finishes to read data from an established telnet connection, independent of success/failure
	      void readComplete(const boost::system::error_code&, size_t);
              /// The method closes a socket
              void closeSocket(void);
              /// The method begins to write data to an established telnet connection
              void writeStart(); 
              /// The method finishes to write data to an established telnet connection, independent of success/failure
              void writeComplete(const boost::system::error_code&);
              /// The method fills the write buffer and initializes the transmission
              void writeToSocket(const char);

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
