#include "include/Telnet.h"

using namespace log4cxx;
using namespace de::hu_berlin::informatik::metrik::daemon;

LoggerPtr Telnet::logger(Logger::getLogger("de.hu_berlin.informatik.metrik.daemon.telnet"));

/**
 *
 */
Telnet::Telnet(boost::asio::io_service& pIOSservice, tcp::resolver::iterator pEndpointIterator) 
  : mIOService(pIOService), mSocket(pIOService) { 
  connect(pEndpointIterator)
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
  this->mIOSservice.post(boost::bind(&Telnet::writeToSocket, this, pMessage));
}

void Telnet::close(){
  //
  this->mIOSservice.post(boost::bind(&Telnet::closeSocket, this));
}

void Telnet::closeSocket(void){
  //
  this->mSocket.close();
}

void Telnet::writeToSocket(const char pMessage){

}

void Telnet::connect(tcp::resolver::iterator pEndpointIterator){
  //
  tcp::endpoint endpoint = *pEndpointIterator;
  //
  socket_.async_connect(endpoint, boost::bind(&telnet::connect_complete, this,
				boost::asio::placeholders::error, ++pEndpointIterator));
}

void Telnet::connectComplete(){
  if(!error){
    read_start();
  }else if(endpoint_iterator != tcp::resolver::iterator()){
    this->socketClose();
    connect_start(endpoint_iterator);
  }
}
