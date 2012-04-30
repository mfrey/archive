#ifndef _CONFIGURATION_H_ 
#define _CONFIGURATION_H_

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

#include "WatchDescriptorEntry.h"

#include <unistd.h>
#include <boost/algorithm/string.hpp>

#include "log4cxx/logger.h"

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Configuration {
            private: 
              static log4cxx::LoggerPtr mLogger;
              const char *mOptions;
              /// The map stores the configuration options set in the configuration file
              std::map<std::string,std::string> mConfiguration;
              /// This is a temporary method for printing the content of an array
              void __dump(std::vector<std::string>);
              /// The method tries to find a configuration option in the options map
              bool findConfigurationOption(std::string);
              /// The method returns an option set in the configuration file
              std::string getOption(std::string);

            public:
              Configuration();
              Configuration(int, char **);
              ~Configuration();
              std::string getLoggingFile();
              std::string getConfigurationFile();
              std::string getFilesToWatch();
              std::string getDirectoriesToWatch();
              void parseOptions(int, char **);
              void readConfigurationFile();
              void setConfigurationFile(std::string);
              std::vector<WatchDescriptorEntry> getWatchDescriptorEntries();
              void parseConfigurationFileLine(std::vector<std::string>);
          };
        }
      }
    }
  }
}

#endif
