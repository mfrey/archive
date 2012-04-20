#include "include/TelnetActor.h"

TelnetActor::TelnetActor(char *pHostName, char *pPort) : mHostName(pHostName), mPort(pPort) {
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
  /// Set local modes
  /*
  this->mSettings.c_lflag &= (~ICANON);
  this->mSettings.c_lflag &= (~ISIG); 
  */
  /// TODO: Verify if that's correc
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
    boost::thread t(boost::bind(&boost::asio::io_service::run, &(this->mIOService)));
    /// TODO
    while(1){
      // TODO
    }
    /// Close the telnet connection 
    telnet.close();
    /// Join the thread
    
  }catch(exception& e){

  }

}
