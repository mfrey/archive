#include "include/TelnetSocket.h"

using namespace std;
using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr TelnetSocket::mLogger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.telnetsocket"));

/**
 * 
 */
TelnetSocket::TelnetSocket(boost::asio::io_service& pIOService, tcp::resolver::iterator pEndpointIterator) 
  : mIOService(pIOService), mSocket(pIOService) { 
  /// Set the type of the terminal
  this->mTerminalType = string("DUMB");
  LOG4CXX_TRACE(mLogger, "set terminal type to " << this->mTerminalType);
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
TelnetSocket::~TelnetSocket(){
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
void TelnetSocket::write(string pMessage){
  LOG4CXX_TRACE(mLogger, "prepare to write message to socket");
  /// Writing the message is delegated to the method writeToSocket
  this->mIOService.post(boost::bind(&TelnetSocket::writeToSocket, this, pMessage));
}

void TelnetSocket::writeToSocket(string pMessage){
   // Check if write buffer is empty
   bool progress = !this->mWriteBuffer.empty();
   LOG4CXX_TRACE(mLogger, "write buffer is empty (true/false) " << progress);
   // Add message to write buffer queue
   this->mWriteBuffer.pushBack(pMessage);
   // Write message if buffer is empty
   if(!progress){
     LOG4CXX_TRACE(mLogger, "start to write data to socket");
     writeStart();
   }
}

void TelnetSocket::__hexdump(string s){
 size_t n=0;


        for(; n < s.size(); ++n)
        {
            if((n%16) == 0)
                fprintf(stdout,"\n    ");
            fprintf(stdout,"%02x:",(unsigned char) s.at(n));
        }
        fprintf(stdout,"\n");
} 	  

string TelnetSocket::__hex_dump(const char *title, string s){
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
void TelnetSocket::readStart(void){
  LOG4CXX_TRACE(mLogger, "will read data from socket");
  this->mSocket.async_read_some(boost::asio::buffer(mBuffer, this->mBufferSize),
    boost::bind(&TelnetSocket::readComplete, this, boost::asio::placeholders::error,
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
void TelnetSocket::readComplete(const boost::system::error_code& pError, size_t pTransferredBytes){
  if(!pError){
    string message = this->unsignedCharToString(this->mBuffer);
    __hexdump(message);

    LOG4CXX_TRACE(mLogger, "read " << hex << message << " from socket " << __hex_dump(NULL, message));
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
void TelnetSocket::connect(tcp::resolver::iterator pEndpointIterator){
  ///
  tcp::endpoint endpoint = *pEndpointIterator;
  ///
  mSocket.async_connect(endpoint, boost::bind(&TelnetSocket::connectComplete, this,
    boost::asio::placeholders::error, ++pEndpointIterator));
}

/**
* The method is called via the connect() method if a telnet connection could be established
* or if establishing a connection failed.
*
* @param pError If an error has occured it is set accordingly. The parameter indicates the type of the error
* @param pEndpointIterator
*/
void TelnetSocket::connectComplete(const boost::system::error_code& pError, tcp::resolver::iterator pEndpointIterator){
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
void TelnetSocket::closeSocket(void){
  // Try to close the open socket
  this->mSocket.close();
}

/**
* The method closes the socket of an established telnet
* connection.
*/
void TelnetSocket::close(){
  /// Closing the socket is delegated to the method closeSocket
  this->mIOService.post(boost::bind(&TelnetSocket::closeSocket, this));
}

/**
 * The method writes asynchronously data to an established telnet connection. If
 * the operation completes or fails, the method writeComplete() is called.
 */
void TelnetSocket::writeStart(){
  /// A variable to read from the (concurrent) deque
  string write;
  /// Fetch the first element in the (concurrent) deque
  this->mWriteBuffer.front(write);
  /// Write the element
  LOG4CXX_TRACE(mLogger, "will write " << write << " to socket");
  boost::asio::async_write(this->mSocket, boost::asio::buffer(&(write), write.size()),
    boost::bind(&TelnetSocket::writeComplete, this, boost::asio::placeholders::error));
}

/**
 * The method is called if writing asynchronously data to an established telnet 
 * connection has completed or failed. The method is called via the writeStart()
 * method.
 *
 * @param pError If an error has occured it is set accordingly. The parameter indicates the type of the error
 */
void TelnetSocket::writeComplete(const boost::system::error_code& pError){
  LOG4CXX_TRACE(mLogger, "wrote data to socket");
  if(!pError){ 
    string write;
    LOG4CXX_TRACE(mLogger, "prepare to remove data from write queue");
    this->mWriteBuffer.waitAndPopFront(write); 
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
 * The method tries to process a given input of data received via telnet. TelnetSocket
 * uses different commands and options as defined in the header of the class. 
 * However, at present not all commands and options are supported.
 * 
 * @param pTransferredBytes The number of bytes which will be processed
 */
void TelnetSocket::handle(size_t pTransferredBytes){
  LOG4CXX_TRACE(mLogger, "try to process " << pTransferredBytes << " bytes of data");

  // Get a chunk of data and store it in a temporary buffer
  unsigned char *buffer = new unsigned char[pTransferredBytes];
  /// Save the content of the internal (receive) buffer in the local buffer 
  memcpy(buffer, this->mBuffer, pTransferredBytes);
  /// Copy content of temporary buffer in global internal read buffer
  for(unsigned int i = 0; i < pTransferredBytes; i++){
    this->mInternalRead.pushBack(buffer[i]);
  }
  /// Delete the previously created buffer
  delete[] buffer;
  
  /// A value to compare
  unsigned char byte;
  /// Get the front element
  this->mInternalRead.front(byte);
  
  for(int i = 0; i < this->mInternalRead.size(); i++){
    // If the first byte is a IAC byte, the following bytes are commands (and options)
    if(((int)byte) == IAC){
      LOG4CXX_TRACE(mLogger, "read IAC byte, prepare to handle commands/options");
      // Remove the first element (IAC) and do nothing with it
      this->mInternalRead.waitAndPopFront(byte);
      // Get the element after the first element
      this->mInternalRead.waitAndPopFront(byte);
      LOG4CXX_TRACE(mLogger, "will command with of type (byte representation)" << ((int)byte));
      // Pass the element to the handleCommand() method
      this->handleCommand(((int)byte)); 
    }else{
      LOG4CXX_TRACE(mLogger, "prepare to handle data");
      // Pass over the data to the appropiate method
      handleData(pTransferredBytes);
    }     
  }
}

/**
 * The method copies data from the internal read buffer to the
 * external read buffer. The paramter determines the number of
 * bytes which will be copied.
 *
 * @param pSize The number of bytes which will be copied from one buffer to another buffer
 */
void TelnetSocket::handleData(int pSize){
  stringstream result;
  unsigned char byte;

  for(int i = 0; i < pSize; i++){
    this->mInternalRead.waitAndPopFront(byte);
    LOG4CXX_TRACE(mLogger, "read byte " << byte);
    result << byte; 
  }
  LOG4CXX_TRACE(mLogger, "pushed " << pSize << " bytes of data to the external read buffer");
  this->mRead.pushBack(result.str());
}

void TelnetSocket::handleOption(int pCommand, int pOption){
  LOG4CXX_TRACE(mLogger, "try to handle command " << (int)pCommand << " with option "  << (int)pOption);

  if(pCommand == DO){ 
    if(pOption == TTYPE){
      LOG4CXX_TRACE(mLogger, "option is terminal type (TTYPE)");
      this->sendTerminalType();
    }else if(pOption == TSPEED){
      LOG4CXX_TRACE(mLogger, "option is terminal speed (TSPEED)");
      this->sendTerminalSpeed();
    }
  }
/*

  /// check if this is a 'DO' request 
  if(pCommand == DO){ 
    LOG4CXX_TRACE(mLogger, "command is a DO request");
    /// check if the option is supported
    if(!(this->isSupportedLocalOption(pOption))){
      this->sendOption(WONT, pOption, true);
    }else{
      this->sendOption(WILL, pOption, false);
*/
 //     if(pOption == NAWS){
        /**
         * Usually the size of the terminal window should not be fix. Since, this
         * class only provides a rough implementation and there is no visual feedback in terms
         * of a virtual terminal, we set the size of the terminal to static values (it doesn't matter). 
         */
/*
        this->sendWindowSizeNegotiation(132, 43);
      }else if(pOption == TTYPE){
        this->sendTerminalType();
      }else if(pOption == NAOHTD){
        this->sendHorizontalTabDisposition();
      }else{

      }
    }
  }else if(pCommand == DONT){
    LOG4CXX_TRACE(mLogger, "command is a DONT request");
    this->sendOption(WONT, pOption, false);
  /// check if this is a 'WILL' request
  }else if(pCommand == WILL){
    LOG4CXX_TRACE(mLogger, "command is a WILL request");
    /// check if the option is supported
    if(!(this->isSupportedRemoteOption(pOption))){
      this->sendOption(WONT, pOption, true);
    }else{
      this->sendOption(DO, pOption, false);
    }
  /// check if this is a 'WONT' request and turn of the option
  }else if(pCommand == WONT){
    LOG4CXX_TRACE(mLogger, "command is a WANT request");
    this->sendOption(DONT, pOption, false);
  }else{
 
  }
*/
}

/** 
 * The method puts a byte before plain data, in order to 
 * indicate a telnet server that the upcoming data is 
 * plain data. In particular, the first byte is set to the
 * value of the IAC command. 
 */
void TelnetSocket::writeData(std::string pData){
  // Write data
  writeCommand(pData, IAC);
}

void TelnetSocket::writeCommand(std::string pData, int pCommand){
  std::stringstream stream;
  stream << pCommand;
  // Insert a IAC byte at the beginning of the string
  pData.insert(0, stream.str()); 
  // Write data
  write(pData);
}

void TelnetSocket::sendOption(int pCommand, int pOption, bool flag){
  LOG4CXX_TRACE(mLogger, "prepare to send a IAC message with command " << pCommand << " and option " << pOption);
  // Build up a message consisting of IAC byte, command and option
  unsigned char *message = new unsigned char[3];

  message[0] = IAC;
  message[1] = pCommand;
  message[2] = pOption;

  /// Write data
  this->write(this->unsignedCharToString(message));
  LOG4CXX_TRACE(mLogger, "put IAC message with command " << pCommand << " and option " << pOption << " to write buffer");
  /// Free the message
  delete[] message;
}

bool TelnetSocket::isSupportedLocalOption(int pOption){
  return false;
}

bool TelnetSocket::isSupportedRemoteOption(int pOption){
  return false;
}

void TelnetSocket::handleSubnegotiation(int pOption){
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
 * The method processes the given command
 */
void TelnetSocket::handleCommand(int pCommand){
  LOG4CXX_TRACE(mLogger, "handle command of type (byte representation) " << pCommand);
  unsigned char option;

  switch(pCommand){
    case DO: 
      // TODO
      this->mInternalRead.waitAndPopFront(option);
      LOG4CXX_TRACE(mLogger, "command is of type DO and option is (byte representation) " << (int)option);
      handleOption(pCommand, option);
    case DONT: 
      //this->mInternalRead.waitAndPopFront(option);
      // TODO
      //LOG4CXX_TRACE(mLogger, "command is of type DONT and option is (byte representation) " << (int)option);
    case IAC:
      LOG4CXX_TRACE(mLogger, "command is of type IAC");
      this->mInternalRead.waitAndPopFront(option);
      // TODO: if that is really smart
      handleCommand(option);
      break;
    case SB:
      this->mInternalRead.waitAndPopFront(option);
      /// Handle command 'SB' 
      LOG4CXX_TRACE(mLogger, "command is of type SB and option is (byte representation) " << (int)option);
      this->handleSubnegotiation((int)option);
      break;
    case WILL:
      /// Read option
      this->mInternalRead.waitAndPopFront(option);
      LOG4CXX_TRACE(mLogger, "command is of type WILL and options is (byte representation) " << (int)option);
      /// Handle command 'WILL' 
      this->handleOption(pCommand, (int)option);
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
void TelnetSocket::sendTerminalType(){
  LOG4CXX_TRACE(mLogger, "prepare to send terminal type (TTYPE)");
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

  LOG4CXX_TRACE(mLogger, "write terminal type (TTYPE) message to write buffer");
  /// Write data
  this->write(this->unsignedCharToString(message));
  /// Free buffer
  delete[] message;
}

void TelnetSocket::sendTerminalSpeed(){
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
  this->write(this->unsignedCharToString(message));
  // Free buffer
  delete[] message;
}

void TelnetSocket::sendHorizontalTabDisposition(){
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
  this->write(this->unsignedCharToString(message));
  // Free buffer
  delete[] message;
}

/**
 * The method transforms an unsigned char to a string using a
 * string stream.
 *
 * @param pBuffer The unsigned char array which should be transformed into a string
 * @return stream The string representation of the unsigned char array
 */
string TelnetSocket::unsignedCharToString(unsigned char *pBuffer){
  /// Create a string stream
  stringstream stream;
  /// Write the buffer into the stream
  stream << pBuffer;
  /// Return a string representation of the string stream
  return stream.str();
  /// TODO: There might be an issue regarding '\0' and string streams (check!)
}

void TelnetSocket::sendWindowSizeNegotiation(int pWidth, int pHeight){
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
  this->write(this->unsignedCharToString(message));
  // Free buffer
  delete[] message;
}
