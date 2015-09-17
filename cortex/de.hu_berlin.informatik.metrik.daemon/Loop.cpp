#include "include/Loop.h"

using namespace log4cxx;

Loop::Loop(){}

Loop::Loop(Actor pActor, Sensor pSensor):mActor(pActor),mSensor(pSensor){

}

Loop::~Loop(){}
