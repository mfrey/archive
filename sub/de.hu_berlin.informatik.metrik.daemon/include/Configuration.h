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
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

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
              /// The method reads a line of a configuration file and sets the corresponding options
              void parseConfigurationFileLine(std::vector<std::string>);
              ///
              std::vector<std::string> splitString(std::string);

            public:
              Configuration();
              /// The constructor reads a configuration (by calling th corresponding methods of the class)
              Configuration(std::string);
              Configuration(int, char **);
              ~Configuration();
              /// The method returns the location of the logging file
              std::string getLoggingFile();
              /// The method returns the location of the configuration file
              std::string getConfigurationFile();
              /// The method returns a list of file which should be watched 
              std::vector<std::string> getFilesToWatch();
              /// The method returns a list of directories which should be watched 
              std::vector<std::string> getDirectoriesToWatch();
              void parseOptions(int, char **);
              /// The method reads a configuration file
              void readConfigurationFile();
              /// The method sets the name (full path) of the configuration file
              void setConfigurationFile(std::string);
              /// The method enables the log4cxx logging
              void enableLogging(void);
              /// The method returns a list of files/directories to watch (represented as watch descriptors)
              std::vector<WatchDescriptorEntry> getWatchDescriptorEntries();
              /// The method returns a list of files and directories which should be watched
              std::vector<std::string> getEntries();
          };
        }
      }
    }
  }
}

#endif
