#ifndef _CONFIGURATION_H_ 
#define _CONFIGURATION_H_

#include <string>

#include "settings.h"

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Configuration {
            private: 
              const char *mOptions;
              ConfigurationSetting mConfiguration;
              std::string mLoggingFile;
              std::string mConfigurationFile;

            public:
              Configuration(int, char **);
              ~Configuration();
              std::string getLoggingFile();
              void parseOptions(int, char **);
              void readConfigurationFile();
          };
        }
      }
    }
  }
}

#endif
