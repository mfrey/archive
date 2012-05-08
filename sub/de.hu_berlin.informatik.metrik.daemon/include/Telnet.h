#ifndef _TELNET_H_
#define _TELNET_H__

#include "Actor.h"
#include "Deque.h"

#include <stdio.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "log4cxx/logger.h"


using boost::asio::ip::tcp;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          /**
           * The class provides a rough and incomplete implementation of the telnet protocol
           * as specified in RFC 854.
           */
          class Telnet {
            private: 
              /// An internal data structure for holding received data in order to build up blocks
	      Deque<std::string> mRead;
	      /// An instance of the log4cxx logger
	      static log4cxx::LoggerPtr mLogger;
	      // A deque for writing data
	      Deque<std::string> mWriteBuffer; 
	      /// Size of the buffer
	      static const int mBufferSize = 128;
	      /// A 'handler' for asynchronous IO
	      boost::asio::io_service& mIOService;
	      void __hexdump(const char*, std::string);
	      std::string __hex_dump(const char*, std::string);

	      /// The socket the telnet the instance is connected to
	      tcp::socket mSocket;
	      /// A buffer for writing data
	      unsigned char mBuffer[mBufferSize];

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
		 /**
		  * Command indicates the desire to begin performing, or confirmation that you are now 
		  * perfoming an indicated option
		  */
		 WILL  = 251,
		 /**
		  * Command indicates the refusal to begin performing, or confirmation that you are now 
		  * perfoming an indicated option
		  */
		 WONT  = 252
	      };

              /**
               * The enumeration holds different telnet options as specifid in multiple RFCs. The
	       * corresponding is set as comment above the option.
	       **/
	      enum TelnetOptions {
		 /// Binary Transmission, RFC 856
		 BT     = 0,
		 /// Echo, RFC 857
		 ECH    = 1,
		 /// Reconnection, NIC 50005
		 R      = 2, 
		 /// Suppress Go Ahead, RFC 858
		 SGA    = 3, 
		 /// Approx Message Size Negotiation, ETHERNET
		 AMSN   = 4, 
	         /// Status, RFC 859
		 STAT   = 5, 
		 /// Timing Mark, RFC 860
		 TM     = 6, 
		 /// Remote Controlled Trans and Echo, RFC 726
		 RCTE   = 7, 
		 /// Output Line Width, NIC 50005
		 OLW    = 8, 
		 /// Output Page Size, NIC 50005
		 OPS    = 9, 
		 /// Output Carriage-Return Disposition, RFC 652
		 OCRD   = 10, 
	         /// Output Horizontal Tab Stops, RFC 653
		 OHTS   = 11, 
		 /// Output Horizontal Tab Disposition, RFC 654
		 NAOHTD = 12, 
		 /// Output Formfeed Disposition, RFC 655
		 OFD    = 13, 
		 /// Output Vertical Tabstops, RFC 656
		 OVT    = 14, 
		 /// Output Vertical Tab Disposition, RFC 657
		 OVTD   = 15, 
		 /// Output Linefeed Disposition, RFC 658
		 OLD    = 16,
		 /// Extended ASCII, RFC 698
		 EA     = 17,
		 /// Logout, RFC 727
		 LOGO   = 18,
		 /// Data Entry Terminal, RFC 1043 and RFC 732
		 DET    = 20,
		 /// Send Location, RFC 749
		 SL     = 23,
		 /// Terminal Type
		 TTYPE  = 24,
		 /// Output Marking, RFC 933 
		 OM     = 27,
		 /// Negotiate Window Size
		 NAWS   = 31,
		 ///
		 TSPEED = 32
	      };
	      /// An array of bools which holds the supported local operations
	      bool *mSupportedLocalOptions;
	      /// An array of bools which holds the supported remote operations
	      bool *mSupportedRemoteOptions;
	      /// The 'type' of the terminal
	      std::string mTerminalType;

	      /// The method begins to open a telnet connection
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
	      void writeToSocket(std::string);

            public:
	      /// The constructor of the telnet class
	      Telnet(boost::asio::io_service&, tcp::resolver::iterator);              
	      /// The destructor of the telnet class
	      ~Telnet();
	      /// The method initializes a telnet connection
	      void open();
	      /// The method writes a string via an established telnet connection
	      void write(std::string);
	      /// The method closes the telnet connection
	      void close();

	      void writeData(std::string);
	      void writeCommand(std::string, int);

	      void handle(size_t);
	      void handleCommand(int);
	      void handleOption(int, int);
	      void sendOption(int, int, bool);
	      bool isSupportedRemoteOption(int);
	      bool isSupportedLocalOption(int);

	      void handleSubnegotiation(int);
	      void handleData(unsigned char*);
              // The method sends the terminal type
              void sendTerminalType(void);
              // The method negotiates the terminal speed
              void sendTerminalSpeed(void);
              // The method negotiates the the horizontal tab disposition
              void sendHorizontalTabDisposition(void);
              // The method negotiate the window size 
              void sendWindowSizeNegotiation(int, int);
          };
        }
      }
    }
  }
}

#endif
