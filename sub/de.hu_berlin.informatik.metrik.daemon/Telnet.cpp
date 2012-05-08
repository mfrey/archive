#include "include/Telnet.h"

using namespace std;
using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr Telnet::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.telnet"));

/**
 * 
 */
Telnet::Telnet(boost::asio::io_service& pIOService, tcp::resolver::iterator pEndpointIterator) 
  : mIOService(pIOService), mSocket(pIOService) { 
  /// Set the type of the terminal
  this->mTerminalType = string("DUMB");
  /// Initialize local options array
  this->mSupportedLocalOptions = new bool[64];
  /// Initialize remote options array
  this->mSupportedRemoteOptions = new bool[64];
  /// Connect
  connect(pEndpointIterator);
}

/**
 *
 */
Telnet::~Telnet(){
  // Check if the socket is still open
  if(mSocket.is_open()){
    // Close the socket
    this->closeSocket(); 
  }
  // Free the local option array
  delete[] this->mSupportedLocalOptions;
  // Free the remote option array
  delete[] this->mSupportedRemoteOptions;
}

/**
 * The methods writes a string via an established telnet
 * connection.
 *
 * @param pMessage The string which should be written
 */
void Telnet::write(string pMessage){
  /// Writing the message is delegated to the method writeToSocket
}


void Telnet::__hexdump(const char *title, string s){
 size_t n=0;

    fprintf(stdout,"%s",title);

        for(; n < s.size(); ++n)
        {
            if((n%16) == 0)
                fprintf(stdout,"\n    ");
            fprintf(stdout,"%02x:",(unsigned char) s.at(n));
        }
        fprintf(stdout,"\n");
} 	  

string Telnet::__hex_dump(const char *title, string s){
  stringstream result;

  for(size_t i = 0; i < s.size(); i++){
    result << internal << setw(2) << setfill('0') << hex << (int) s.at(i); 
  }

  return result.str();
}

/**
* The method reads asynchronously data from an established telnet connection. If
* the operation completes or fails, the method readComplete() is called.
*/
void Telnet::readStart(void){
  LOG4CXX_TRACE(mLogger, "will read data from socket");
  this->mSocket.async_read_some(boost::asio::buffer(mBuffer, this->mBufferSize),
    boost::bind(&Telnet::readComplete, this, boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

/**
 * The method is called if reading asynchronously data from an established telnet 
 * connection has completed or failed. The method is called via the readStart()
 * method.
 *
 * @param pError If an error has occured it is set accordingly. The parameter indicates the type of the error
 * @param pTransferredBytes TODO
 */
void Telnet::readComplete(const boost::system::error_code& pError, size_t pTransferredBytes){
  if(!pError){
    stringstream data;
    data << this->mBuffer;
    __hexdump("read ", data.str());

    LOG4CXX_TRACE(mLogger, "read " << hex << data.str() << " from socket " << __hex_dump(NULL, data.str()));
    LOG4CXX_TRACE(mLogger, "bytes " << pTransferredBytes);
    // Parse received data
    this->handle(pTransferredBytes);
    // TODO: write read out, e.g. cout
    readStart();
  }else{
    LOG4CXX_FATAL(mLogger, "error " << pError.value() << " of type "  << pError.category().name() << " occured while reading data from socket");
    boost::system::system_error reason(pError);
    LOG4CXX_FATAL(mLogger, "reason " << reason.what());
    this->closeSocket();
  }
}

/**
* The method establishes a telnet connection. If the connection was established or failed
* the connectComplete() method is called.
*
* @param pEndpointIterator
*/
void Telnet::connect(tcp::resolver::iterator pEndpointIterator){
  ///
  tcp::endpoint endpoint = *pEndpointIterator;
  ///
  mSocket.async_connect(endpoint, boost::bind(&Telnet::connectComplete, this,
    boost::asio::placeholders::error, ++pEndpointIterator));
}

/**
* The method is called via the connect() method if a telnet connection could be established
* or if establishing a connection failed.
*
* @param pError If an error has occured it is set accordingly. The parameter indicates the type of the error
* @param pEndpointIterator
*/
void Telnet::connectComplete(const boost::system::error_code& pError, tcp::resolver::iterator pEndpointIterator){
  if(!pError){
    LOG4CXX_TRACE(mLogger, "start to read data from socket");
    readStart();
  }else if(pEndpointIterator != tcp::resolver::iterator()){
    LOG4CXX_FATAL(mLogger, "error " << pError.value() << " of type " << pError.category().name() << " occured while reading data from socket");
    boost::system::system_error reason(pError);
    LOG4CXX_FATAL(mLogger, "reason " << reason.what());
    this->closeSocket();
    connect(pEndpointIterator);
  }
}

/**
* The method closes an socket.
*/
void Telnet::closeSocket(void){
  // Try to close the open socket
  this->mSocket.close();
}

/**
 * The method writes asynchronously data to an established telnet connection. If
 * the operation completes or fails, the method writeComplete() is called.
 */
void Telnet::writeStart(){
  /// A variable to read from the (concurrent) deque
  string write;
  /// Fetch the first element in the (concurrent) deque
  this->mWriteBuffer.front(write);
  /// Write the element
  LOG4CXX_TRACE(mLogger, "will write " << write << " to socket");
  boost::asio::async_write(this->mSocket, boost::asio::buffer(&(write), write.size()),
    boost::bind(&Telnet::writeComplete, this, boost::asio::placeholders::error));
}

/**
 * The method is called if writing asynchronously data to an established telnet 
 * connection has completed or failed. The method is called via the writeStart()
 * method.
 *
 * @param pError If an error has occured it is set accordingly. The parameter indicates the type of the error
 */
void Telnet::writeComplete(const boost::system::error_code& pError){
  if(!pError){ 
    LOG4CXX_TRACE(mLogger, "prepare to remove data from write queue");
// TODO    this->mWriteBuffer.waitAndPopFront(); 
    LOG4CXX_TRACE(mLogger, "removed data from write queue");
    if(!(this->mWriteBuffer.empty())){
      writeStart(); 
    }
  }else{
    LOG4CXX_FATAL(mLogger, "error " << pError.value() << " of type "  << pError.category().name() << " occured while reading data from socket");
    boost::system::system_error reason(pError);
    LOG4CXX_FATAL(mLogger, "reason " << reason.what());
    this->closeSocket();
  }
}

/**
 * The method tries to process a given input of data received via telnet. Telnet
 * uses different commands and options as defined in the header of the class. 
 * However, at present not all commands and options are supported.
 * 
 * @param pTransferredBytes The number of bytes which will be processed
 */
void Telnet::handle(size_t pTransferredBytes){
  LOG4CXX_TRACE(mLogger, "try to process" << pTransferredBytes << " bytes of data");
  /// Create a private buffer
  unsigned char *buffer = new unsigned char[pTransferredBytes];
  /// Save the content of the internal (receive) buffer in a local buffer 
  memcpy(buffer, this->mBuffer, pTransferredBytes);

  // If the first byte is a IAC byte, the following bytes are commands (and options)
  if(((int)buffer[0]) == IAC){
    LOG4CXX_TRACE(mLogger, "prepare to handle commands/options");
    
  }else{
    LOG4CXX_TRACE(mLogger, "prepare to handle data");
    // Pass over the data to the appropiate method
    handleData(buffer);
  }     
 
  // Delete the previosusly created buffer
  delete[] buffer;
}

void Telnet::handleData(unsigned char *mBuffer){
  if(mBuffer != NULL){
    stringstream stream;
    stream << mBuffer;
    this->mRead.pushFront(stream.str());
  }else{
    LOG4CXX_FATAL(mLogger, "buffer points to null");
  }
}

void Telnet::handleOption(int pCommand, int pOption){
  /// check if this is a 'DO' request 
  if(pCommand == DO){
    /// check if the option is supported
    if(!(this->isSupportedLocalOption(pOption))){
      this->sendOption(WONT, pOption, true);
    }else{
      this->sendOption(WILL, pOption, false);

      if(pOption == NAWS){
        /**
         * Usually the size of the terminal window should not be fix. Since, this
         * class only provides a rough implementation and visual feedback in terms
         * of a real terminal, we set the size static (it doesn't matter). 
         */
        this->sendWindowSizeNegotiation(132, 43);
      }else if(pOption == TTYPE){
        this->sendTerminalType();
      }else if(pOption == NAOHTD){
        this->sendHorizontalTabDisposition();
      }else{

      }
    }
  }else if(pCommand == DONT){
    this->sendOption(WONT, pOption, false);
  /// check if this is a 'WILL' request
  }else if(pCommand == WILL){
    /// check if the option is supported
    if(!(this->isSupportedRemoteOption(pOption))){
      this->sendOption(WONT, pOption, true);
    }else{
      this->sendOption(DO, pOption, false);
    }
  /// check if this is a 'WONT' request and turn of the option
  }else if(pCommand == WONT){
    this->sendOption(DONT, pOption, false);
  }else{
 
  }
}


/** 
 * The method puts a byte before plain data, in order to 
 * indicate a telnet server that the upcoming data is 
 * plain data. In particular, the first byte is set to the
 * value of the IAC command. 
 */
void Telnet::writeData(std::string pData){
  // Write data
  writeCommand(pData, IAC);
}

void Telnet::writeCommand(std::string pData, int pCommand){
  std::stringstream stream;
  stream << pCommand;
  // Insert a IAC byte at the beginning of the string
  pData.insert(0, stream.str()); 
  // Write data
  write(pData);
}

void Telnet::sendOption(int pCommand, int pOption, bool flag){

}

bool Telnet::isSupportedLocalOption(int pOption){
  return false;
}

bool Telnet::isSupportedRemoteOption(int pOption){
  return false;
}

void Telnet::handleSubnegotiation(int pOption){
  /// Is option of type 'terminal type'
  if(pOption == TTYPE){
    LOG4CXX_TRACE(mLogger, "handle subnegotiation: send terminal type");
    this->sendTerminalType();
  /// Is option of type 'terminal speed'
  }else if(pOption == TSPEED){
    LOG4CXX_TRACE(mLogger, "handle subnegotiation: send terminal speed");
    this->sendTerminalSpeed();
  /// Operation not supported
  }else{
    LOG4CXX_TRACE(mLogger, "handle subnegotiation: operation not supported");
  }
}


/**
 *
 */
void Telnet::handleCommand(int pCommand){
  int option = -1;

  switch(pCommand){
    case DO: /** do nothing*/
    case DONT: /** do nothing*/
    case IAC:
      // TODO: handleData(pCommand);
      break;
    case SB:
      /// Read option TODO: FIXME
     // option = this->mRead.tryPopFront();
      /// Handle command 'SB' 
      this->handleSubnegotiation(option);
      break;
    case WILL:
      /// Read option
     // option = this->mRead.tryPopFront();
      /// Handle command 'WILL' 
      this->handleOption(pCommand, option);
      break;
    case WONT: /** do nothing*/
    default:
      return;
  }

  // Send data (if initialized for the first time)
}

/** 
 * The method prepares a message for setting the terminal type. 
 */
void Telnet::sendTerminalType(){
  // A variable in order to fill the message string
  int i = 0;
  // The size calculates as follows: length of terminal type + six control bytes (IAC, SB, TTYPE, IAC, IAC, SE) 
  int size = this->mTerminalType.size() + 6; 
  // The buffer which will be filled and sent
  unsigned char *message = new unsigned char[size];

  /// Indicate data will be sent
  message[i++] = IAC;
  /// Write begin of subnegotiation
  message[i++] = SB;
  /// Subnegotiation concerns setting the terminal type
  message[i++] = TTYPE;
  message[i++] = IAC;
  // Set the terminal type
  for(unsigned int j = 0; j < this->mTerminalType.size(); j++){
    message[i++] = (unsigned char)this->mTerminalType.at(j);
  }

  message[i++] = IAC;
  /// Write end of subnegotiation
  message[i++] = SE;

  /// Write data
//  this->mWriteBuffer(string(message));
  /// Free buffer
  delete[] message;
}

void Telnet::sendTerminalSpeed(){
  // A variable in order to fill the message string
  int i = 0;
  // Construct a message
  unsigned char *message = new unsigned char[20]; 

  message[i++] = IAC;
  message[i++] = SB;
  message[i++] = TSPEED;

  /// Set the baud rate to 52000
  message[i++] = 0;
  message[i++] = (unsigned char) '5';
  message[i++] = (unsigned char) '2';
  message[i++] = (unsigned char) '0';
  message[i++] = (unsigned char) '0';
  message[i++] = (unsigned char) '0';
  message[i++] = (unsigned char) ',';
  message[i++] = (unsigned char) '5';
  message[i++] = (unsigned char) '2';
  message[i++] = (unsigned char) '0';
  message[i++] = (unsigned char) '0';
  message[i++] = (unsigned char) '0';

  message[i++] = IAC;
  message[i++] = SE;

  /// Write data
//  this->mWriteBuffer(string(message));
  // Free buffer
  delete[] message;
}

void Telnet::sendHorizontalTabDisposition(){
  // A variable in order to fill the message string
  int i = 0;
  // Construct a message
  unsigned char *message = new unsigned char[20]; 

  message[i++] = IAC;
  message[i++] = SB;
  message[i++] = NAOHTD;
  message[i++] = 0;
  message[i++] = 0;

  message[i++] = IAC;
  message[i++] = SE;

  /// Write data
//  this->mWriteBuffer(string(message));
  // Free buffer
  delete[] message;
}

void Telnet::sendWindowSizeNegotiation(int pWidth, int pHeight){
  // A variable in order to fill the message string
  int i = 0;
  // Construct a message
  unsigned char *message = new unsigned char[14]; 

  message[i++] = IAC;
  message[i++] = SB;
  message[i++] = NAWS;
  // Set the width
  message[i++] = IAC;
  message[i++] = (pWidth >> 8) & 0xFF;
  message[i++] = IAC;
  message[i++] = pWidth & 0xFF;
  message[i++] = IAC;
  // Set the height
  message[i++] = (pHeight >> 8) & 0xFF;
  message[i++] = IAC;
  message[i++] = pHeight & 0xFF;

  message[i++] = IAC;
  message[i++] = SE;
  /// Write data
//  this->mWriteBuffer(string(message));
  // Free buffer
  delete[] message;
}
