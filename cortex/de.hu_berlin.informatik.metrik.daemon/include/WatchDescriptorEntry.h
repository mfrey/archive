#ifndef _WATCHDESCRIPTORENTRY_H_
#define _WATCHDESCRIPTORENTRY_H_ 

#include <iostream>
#include <stdint.h>
#include "log4cxx/logger.h"


namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class WatchDescriptorEntry {
            private:
              /** a logger instance */
              static log4cxx::LoggerPtr logger;
              /** the watch descriptor */
              int mWd;
              /** the name of the file/directory which will be watched */
              std::string name;
              /** the mask of the watch descriptor */
              uint32_t mask;

            public:
	      WatchDescriptorEntry();
              WatchDescriptorEntry(int, uint32_t, std::string);
              ~WatchDescriptorEntry();
              std::string getName();
              int getId();
              void setId(int);
              void setName(std::string);
              uint32_t getMask();
              void setMask(uint32_t);
          };
        }
      }
    }
  }
}

#endif
