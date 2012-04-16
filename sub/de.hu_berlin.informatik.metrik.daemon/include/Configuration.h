#ifndef _CONFIGURATION_H_ 
#define _CONFIGURATION_H_

#include <string>
#include <fstream>
#include <iostream>

#include <vector>

#include "WatchDescriptorEntry.h"

#include "ConfigurationSettings.h"

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Configuration {
            private: 
              const char *mOptions;
              ConfigurationSettings mConfiguration;
              std::string mLoggingFile;
              const char *mConfigurationFile;

            public:
              Configuration();
              Configuration(int, char **);
              ~Configuration();
              std::string getLoggingFile();
              void parseOptions(int, char **);
              void readConfigurationFile();
              std::vector<WatchDescriptorEntry> getWatchDescriptorEntries();
          };
        }
      }
    }
  }
}

#endif
