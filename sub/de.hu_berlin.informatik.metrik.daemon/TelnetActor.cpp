#include "include/TelnetActor.h"

using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr TelnetActor::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.telnetactor"));

/**
 *
 */
TelnetActor::TelnetActor(string pHostName, string pPort) : mHostName(pHostName), mPort(pPort) {
  /// Set up the terminal interface
  this->setup();
}

TelnetActor::~TelnetActor(){

}

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

/**
 * The ...
 */
void TelnetActor::run(void){
  try{
    ///
    ///
    LOG4CXX_TRACE(mLogger, "try to connect to host " << this->mHostName << " and port "  << this->mPort);
    ///
    /// Try to establish the connection
    Telnet telnet();
    /// The data/command which will be transmitted
    std::string command;

    while(1){
      LOG4CXX_TRACE(mLogger, "wait for data to write to the connection");
      /// Read from write buffer
      this->mWriteBuffer.waitAndPopFront(command);
      LOG4CXX_TRACE(mLogger, "received data: " << command);
      /// If the command '#stop' is received, the thread will be shutdown
      if(command.compare("#stop") == 0){
        LOG4CXX_TRACE(mLogger, "received stop command");
        break;
      }else{
        LOG4CXX_TRACE(mLogger, "write data " << command << " to the socket");
        /// Write data to the established telnet connection
        //telnet.write(command);
      }
    }
    LOG4CXX_TRACE(mLogger, "waiting for data to write stopped");
    /// Close the telnet connection 
   // telnet.close();
    LOG4CXX_TRACE(mLogger, "closed connection");
    /// Join the thread
///    thread.join();    
    LOG4CXX_TRACE(mLogger, "joined thread");
  }catch(exception& e){
    string reason = "an exception occurred: " + string(e.what());
    LOG4CXX_FATAL(mLogger, reason);
  }
}

/**
 * New messages which should be transmitted via telnet. Every
 * messages is appended to the write buffer. 
 *
 * @param pMessage The message which should be appended to write buffer
 */
void TelnetActor::send(string pMessage){
  LOG4CXX_TRACE(mLogger, "append message " << pMessage << " to write buffer");
  this->mWriteBuffer.pushBack(pMessage);
}
