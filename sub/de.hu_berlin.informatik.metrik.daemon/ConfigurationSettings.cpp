#include "include/ConfigurationSettings.h"

using namespace de::hu_berlin::informatik::metrik::daemon;

ConfigurationSettings::ConfigurationSettings(){

}

ConfigurationSettings::~ConfigurationSettings(){

}

std::string ConfigurationSettings::getLoggingFile(){
  return this->mLoggingFile;
}

void ConfigurationSettings::setLoggingFile(std::string pLoggingFile){
  this->mLoggingFile = pLoggingFile;
}

