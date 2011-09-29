#include "include/configuration.h"

using namespace de::hu_berlin::informatik::metrik::daemon;

Configuration::Configuration(int pArgc, char **pArgv){
  this->mOptions = "";
}

Configuration::~Configuration(){

}


void Configuration::parseOptions(int pArgc, char **pArgv){
  char c;

  while((c = getopt(pArgc, pArgv, this->mOptions)) != -1){
    switch(c){
      case 'h':
        break;
      case 'c':
        this->mConfigurationFile = optarg;
        break;
      case 'l':
        break;
      default: 
        break;
    }
  }
}

void Configuration::readConfigurationFile(){

}

std::string Configuration::getLoggingFile(){
  return this->mLoggingFile;
}
