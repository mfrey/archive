#include "include/Telnet.h"

using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr Telnet::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.telnet"));

/**
 * 
 */
Telnet::Telnet(boost::asio::io_service& pIOService, tcp::resolver::iterator pEndpointIterator) 
  : mIOService(pIOService), mSocket(pIOService) { 
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
}

/**
 * The methods writes a string via an established telnet
 * connection.
 *
 * @param pMessage The string which should be written
 */
void Telnet::write(string pMessage){
  /// Writing the message is delegated to the method writeToSocket
 this->mIOService.post(boost::bind(&Telnet::writeToSocket, this, pMessage));
}

/**
 * The method closes the socket of an established telnet
 * connection.
 */
void Telnet::close(){
  /// Closing the socket is delegated to the method closeSocket 
  this->mIOService.post(boost::bind(&Telnet::closeSocket, this));
}

/**
 * The method closes an socket.
 */
void Telnet::closeSocket(void){
  // Try to close the open socket
  this->mSocket.close();
}

void Telnet::writeToSocket(string pMessage){
   //
   bool progress = !this->mWriteBuffer.empty(); 
   //
   this->mWriteBuffer.push_back(pMessage); 
   if(!progress){ 
     writeStart();
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
    readStart();
  }else if(pEndpointIterator != tcp::resolver::iterator()){
    this->closeSocket();
    connect(pEndpointIterator);
  }
}

/**
 * The method reads asynchronously data from an established telnet connection. If
 * the operation completes or fails, the method readComplete() is called.
 */
void Telnet::readStart(void){
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
    // TODO: write read out, e.g. cout
    readStart();
  }else{
    this->closeSocket();
  }
}

/**
 * The method writes asynchronously data to an established telnet connection. If
 * the operation completes or fails, the method writeComplete() is called.
 */
void Telnet::writeStart(){
  boost::asio::async_write(this->mSocket, boost::asio::buffer(&(this->mWriteBuffer.front()) ,this->mWriteBuffer.front().size()),
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
    this->mWriteBuffer.pop_front(); 
    if(!(this->mWriteBuffer.empty())){
      writeStart(); 
    }
  }else{
    this->closeSocket();
  }
}
