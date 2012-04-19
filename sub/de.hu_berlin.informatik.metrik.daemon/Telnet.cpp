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

Telnet::~Telnet(){
  // Check if the socket is still open
  if(mSocket.is_open()){
    // Close the socket
    this->closeSocket(); 
  }
}

void Telnet::write(const char pMessage){
  //
  this->mIOService.post(boost::bind(&Telnet::writeToSocket, this, pMessage));
}

void Telnet::close(){
  //
  this->mIOService.post(boost::bind(&Telnet::closeSocket, this));
}

void Telnet::closeSocket(void){
  //
  this->mSocket.close();
}

void Telnet::writeToSocket(const char pMessage){
   //
   bool progress = !write_msgs_.empty(); 
   //
   write_msgs_.push_back(pMessage); 
   //
   if(!progress){ 
     writeStart();
   }
}

void Telnet::connect(tcp::resolver::iterator pEndpointIterator){
  //
  tcp::endpoint endpoint = *pEndpointIterator;
  //
  mSocket.async_connect(endpoint, boost::bind(&Telnet::connectComplete, this,
				boost::asio::placeholders::error, ++pEndpointIterator));
}

void Telnet::connectComplete(const boost::system::error_code& pError, tcp::resolver::iterator pEndpointIterator){
  if(!pError){
    read();
  }else if(pEndpointIterator != tcp::resolver::iterator()){
    this->closeSocket();
    connect(pEndpointIterator);
  }
}

void Telnet::read(void){
  this->mSocket.async_read_some(boost::asio::buffer(read_msg_, this->mBufferSize),
    boost::bind(&Telnet::readComplete, this, boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

void Telnet::readComplete(const boost::system::error_code& pError, size_t pTransferredBytes){
  if(!pError){
    // TODO: write read out, e.g. cout
    read();
  }else{
    this->closeSocket();
  }
}

void Telnet::writeStart(){
  boost::asio::async_write(this->mSocket, boost::asio::buffer(&write_msgs_.front(), 1),
    boost::bind(&Telnet::writeComplete, this, boost::asio::placeholders::error));
}

void Telnet::writeComplete(const boost::system::error_code& pError){
  if(!pError){ 
    write_msgs_.pop_front(); 
    if(!write_msgs_.empty()){
      writeStart(); 
    }
  }else{
    this->closeSocket();
  }
}
