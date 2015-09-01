#ifndef _LOOP_H_ 
#define _LOOP_H_

#include "Actor.h"
#include "Sensor.h"

#include "log4cxx/logger.h"

using namespace de::hu_berlin::informatik::metrik::daemon;

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class Loop {
            private: 
              static log4cxx::LoggerPtr mLogger;

              Actor mActor;
              Sensor mSensor;

            public:
              Loop();
              Loop(Actor,Sensor);
              ~Loop();
          };
        }
      }
    }
  }
}

#endif
