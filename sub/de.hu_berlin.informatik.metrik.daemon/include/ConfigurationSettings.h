#ifndef _CONFIGURATION_SETTINGS_H_ 
#define _CONFIGURATION_SETTINGS_H_

#include <string>
#include <fstream>
#include <iostream>

#include <vector>

#include "WatchDescriptorEntry.h"

#include <unistd.h>

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class ConfigurationSettings {
            private: 
              std::string mLoggingFile;

            public:
              ConfigurationSettings();
              ~ConfigurationSettings();
              std::string getLoggingFile();
              void setLoggingFile(std::string);
          };
        }
      }
    }
  }
}

#endif
