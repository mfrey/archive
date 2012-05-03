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
              static const int mBufferSize = 128;
              /// A 'handler' for asynchronous IO
              boost::asio::io_service& mIOService;
              /// An instance of the log4cxx logger
              static log4cxx::LoggerPtr mLogger;
              /// The socket the telnet the instance is connected to
              tcp::socket mSocket; 
              /// A buffer for writing data
	      char mBuffer[mBufferSize]; 
              // A deque for reading data

              // A deque for writing data
              deque<string> mWriteBuffer; 

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
              void writeToSocket(string);

              /**
               * The enumeration holds different telnet commands as specifid in RFC 854. For
               * further details of the command please be referred to:
               *
               *   https://www.ietf.org/rfc/rfc854.txt
               **/
              enum TelnetCommands {
                 /// 'Abort Output' command
                 AO    = 245,
                 /// 'Are You There' command
                 AYT   = 246,
                 /// 'Break' command
                 BREAK = 243,
                 /// 'Data Mark' command
                 DM    = 242,
                 /** 
                  * Command indicates the request that the other party perform, or confirmation that you're 
                  * expecting the other party to perform, the indicated option
                  */
                 DO    = 253,
                 /** 
                  * Command indicates the demand that the other party perform, or confirmation that you're 
                  * expecting the other party to perform, the indicated option
                  */
                 DONT  = 254,
                 /// 'Erase Character' command
                 EC    = 247,
                 /// 'Erase Line' command
                 EL    = 248,
                 /// 'Go Ahead' command 
                 GA    = 249,
                 /// Data Byte 255
                 IAC   = 255,
                 /// 'Interrupt Process' command
                 IP    = 244,
                 /// 'No Operation' command
                 NOP   = 241,
                 /// Command indiciates that what follows is subnegotioation of the indicated options
                 SB    = 250,
                 /// Command indicates the end of subnegotiation
                 SE    = 240,
                 /// Command indicates the desire to begin performing, or confirmation that you are now perfoming an indicated option
                 WILL  = 251,
                 /// Command indicates the refusal to begin performing, or confirmation that you are now perfoming an indicated option
                 WONT  = 252
              };

            public:
              /// The constructor of the telnet class
              Telnet(boost::asio::io_service&, tcp::resolver::iterator);
              /// The destructor of the telnet class
              ~Telnet();
              /// The method initializes a telnet connection
              void open();
              /// The method writes a string via an established telnet connection
              void write(string);
              /// The method closes the telnet connection
              void close();

              /// 
              void processTelnetCommand(void);
              void processTelnetData(void);
              void processTelnetSubnegotiation(void);
              void processTelnetOption(void);

              void writeData(std::string);
              void writeCommand(std::string, int);
          };
        }
      }
    }
  }
}

#endif
