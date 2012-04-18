#include "include/ActorTelnet.h"

using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr ActorTelnet::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.actortelnet"));

ActorTelnet::ActorTelnet(){}

ActorTelnet::~ActorTelnet(){}

ActorTelnet::ActorTelnet(struct sockaddr_in pAddress, struct hostent *pHost){}
