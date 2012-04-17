#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

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
          class Controller {
            private: 

            public:
              Controller();
              ~Controller();
          };
        }
      }
    }
  }
}

#endif
