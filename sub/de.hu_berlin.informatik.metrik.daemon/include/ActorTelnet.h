#ifndef _ACTORTELNET_H_
#define _ACTORTELNET_H__

#include "Actor.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "log4cxx/logger.h"

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          class ActorTelnet : Actor {
            private: 
              int mFileDescriptor;
              static log4cxx::LoggerPtr logger;
              struct hostent mHost;
              struct sockaddr_in mAddress;

            public:
	      ActorTelnet(struct sockaddr_in, struct hostent*);
              ActorTelnet();
              void open();
              void close();
              ~ActorTelnet();
          };
        }
      }
    }
  }
}

#endif
