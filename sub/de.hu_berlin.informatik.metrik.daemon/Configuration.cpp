#include "include/Configuration.h"

using namespace de::hu_berlin::informatik::metrik::daemon;

Configuration::Configuration(){

}

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
  std::string line;

  std::ifstream configurationFile;
  // open configuration file
  configurationFile.open(this->mConfigurationFile);
  // check if it is really open
  if(configurationFile.is_open()){
/*
    // read all lines from 
    while(!configurationFile.eof){
      getline(configurationFile, line);
      // tokenize it 

      // pass it to configuration file
//      this->mConfiguration.set();
    }
*/
    configurationFile.close();
  }
}

std::string Configuration::getLoggingFile(){
  return this->mLoggingFile;
}

std::vector<WatchDescriptorEntry> Configuration::getWatchDescriptorEntries(){
 return std::vector<WatchDescriptorEntry>();
}
