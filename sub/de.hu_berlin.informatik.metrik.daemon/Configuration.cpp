#include "include/Configuration.h"

using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr Configuration::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.configuration"));

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
//        this->mConfigurationFile = optarg;
        break;
      case 'l':
        break;
      default: 
        break;
    }
  }
}

void Configuration::setConfigurationFile(std::string pConfigurationFile){
  LOG4CXX_TRACE(mLogger, "set configuration file to " << pConfigurationFile);
  this->mConfiguration.insert(std::pair<std::string, std::string>(std::string("configuration_file"), pConfigurationFile));
}

/**
 * The method reads a configuration file and stores the settings
 * in a ConfigurationSettings object.
 */
void Configuration::readConfigurationFile(){
  std::string configuration = getConfigurationFile();
  // Holds content of a line from a configuration file
  std::string line;
  // Stores contents of a line
  std::vector<std::string> array;
  // A input stream of a file 
  std::ifstream configurationFile;
  // open configuration file
  configurationFile.open(configuration.c_str());

  // check if it is really open
  if(configurationFile.is_open()){
    // read all lines from file until eof
    while(!configurationFile.eof()){
      getline(configurationFile, line);

      // Drop the comment line
      if(!boost::starts_with(line, "#")){
       LOG4CXX_TRACE(mLogger, "read line " << line);
       // tokenize it 
       boost::split(array, line, boost::is_any_of("="));
       LOG4CXX_TRACE(mLogger, "array contains " << array.size() << " item(s)");
       __dump(array);
       // parse the content of the line
       parseConfigurationFileLine(array);
      }
    }
    configurationFile.close();
  }
}

void Configuration::__dump(std::vector<std::string> pArray){
  for(unsigned int i = 0; i < pArray.size(); i++){
    LOG4CXX_TRACE(mLogger, "array[" << i << "] = " << pArray[i]);
  }
}

void Configuration::parseConfigurationFileLine(std::vector<std::string> pLine){
  if(pLine.size() > 1){
    // No additional operation on the arguments of the option are required
    if(pLine.size() == 2){
      this->mConfiguration.insert(std::pair<std::string, std::string>(pLine[0], pLine[1]));
    }
  }
}

bool Configuration::findConfigurationOption(std::string pOption){
  std::map<std::string,std::string>::iterator iterator;
  iterator = this->mConfiguration.find(pOption);
  return (iterator != this->mConfiguration.end();
}

std::string Configuration::getLoggingFile(){
  return getOption("logging_file");
}

std::string Configuration::getConfigurationFile(){
  return getOption("configuration_file");
}

std::string Configuration::getFilesToWatch(){
  return getOption("watch_files");
}

std::string Configuration::getDirectoriesToWatch(){
  return getOption("watch_directories");
}

std::string Configuration::getOption(std::string pOption){
  if(findConfigurationOption(pOption)){
    return this->mConfiguration[pOption];
  }
  return std::string("option_not_set");
}

std::vector<WatchDescriptorEntry> Configuration::getWatchDescriptorEntries(){
 return std::vector<WatchDescriptorEntry>();
}
