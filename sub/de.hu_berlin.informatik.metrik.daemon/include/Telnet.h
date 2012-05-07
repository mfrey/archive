#ifndef _TELNET_H_
#define _TELNET_H__

#include "Actor.h"
#include <stdio.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "log4cxx/logger.h"

#include <curl/curl.h>

using namespace std;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          /**
           * The class
           */
          class Telnet {
            private: 
              /// 
              CURL *mCurl;
              ///
              CURLcode *mCurlcode;

              /// An internal data structure for holding received data in order to build up blocks
              deque<string> mReadBuffer; 
              /// An instance of the log4cxx logger
              static log4cxx::LoggerPtr mLogger;
              // A deque for writing data
              deque<string> mWriteBuffer; 

              void __hexdump(const char*, string);
              string __hex_dump(const char*, string);

            public:
              /// The constructor of the telnet class
              Telnet();
              /// The destructor of the telnet class
              ~Telnet();
              /// The method initializes a telnet connection
              void open();
              /// The method writes a string via an established telnet connection
              void write(string);
              /// The method closes the telnet connection
              void close();

              void writeData(std::string);
              void writeCommand(std::string, int);

          };
        }
      }
    }
  }
}

#endif
