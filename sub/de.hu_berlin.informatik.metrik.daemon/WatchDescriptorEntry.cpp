#include "include/WatchDescriptorEntry.h"

using namespace de::hu_berlin::informatik::metrik::daemon;
using namespace log4cxx;

WatchDescriptorEntry::WatchDescriptorEntry(){

}

WatchDescriptorEntry::WatchDescriptorEntry(int pWd, uint32_t  pMask, std::string pName){
  this->mWd = pWd;
  this->mask = pMask;
  this->name = pName;
}

WatchDescriptorEntry::~WatchDescriptorEntry(){

}

std::string WatchDescriptorEntry::getName(){
  return this->name;
}

void WatchDescriptorEntry::setName(std::string pName){
  this->name = pName;
}

void WatchDescriptorEntry::setMask(uint32_t pMask){
  this->mask = pMask;
}

uint32_t WatchDescriptorEntry::getMask(){
  return this->mask;
}

int WatchDescriptorEntry::getId(){
  return this->mWd;
}

void WatchDescriptorEntry::setId(int pWd){
  this->mWd = pWd;
}
