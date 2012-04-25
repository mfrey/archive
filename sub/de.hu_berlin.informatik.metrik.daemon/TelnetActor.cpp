#include "include/TelnetActor.h"

using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr TelnetActor::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.telnetactor"));

TelnetActor::TelnetActor(string pHostName, string pPort) : mHostName(pHostName.c_str()), mPort(pPort.c_str()) {
  /// Set up the terminal interface
  this->setup();
}

TelnetActor::~TelnetActor(){}

/**
 * The method sets the options for control for the terminal interface. In
 * particular, the settings for the telnet connection.
 */
void TelnetActor::setup(){
  /// Initialize structure for storing previous settings
  termios stored_settings;
  /// Determine the previous settings
  tcgetattr(0, &stored_settings);
  /// Save the previous settings
  this->mSettings = stored_settings;
  /// Set settings
  this->mSettings.c_lflag &= ((~ICANON) | (~ISIG));
  /// Set new attributes
  tcsetattr(0, TCSANOW, &(this->mSettings));
}


void TelnetActor::run(void){
  try{
    ///
    tcp::resolver resolver(this->mIOService);
    ///
    tcp::resolver::query query(this->mHostName, this->mPort);
    ///
    this->mEndpointIterator = resolver.resolve(query);
    /// Try to establish the connection
    Telnet telnet(this->mIOService, this->mEndpointIterator);
    /// The IO service will run as seperate thread 
    boost::thread thread(boost::bind(&boost::asio::io_service::run, &(this->mIOService)));
   
    while(1){
      /// Check if there is data available to write
      /// Acquire lock

      /// Write data

      /// Check if data contains control command

      /// Release lock
/*
      if(this->mWriteBuffer){
        break;
      }
*/
    }
    /// Close the telnet connection 
    telnet.close();
    /// Join the thread
    thread.join();    
  }catch(exception& e){
    string reason = "An exception occurred: " + string(e.what());
    // TODO: Catch exception
    LOG4CXX_FATAL(mLogger, reason);
  }
}

void TelnetActor::send(string pMessage){

}
