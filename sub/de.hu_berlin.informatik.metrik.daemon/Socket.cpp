#include "include/Socket.h"

using namespace de::hu_berlin::informatik::metrik::daemon;
//using namespace log4cxx;

Socket::Socket(){}

Socket::Socket(std::string pHostName, std::string pPort):mHostName(pHostName),mPort(pPort){

}

void Socket::setup(){
  /// Clear the structure with zeros
  memset(&(this->mHints), 0, sizeof(this->mHints));
  /// Set the family to
  this->mHints.ai_family   = AF_UNSPEC;
  /// Set the socket type to stream
  this->mHints.ai_socktype = SOCK_STREAM;
}

Socket::~Socket(){}

int Socket::open(){
  struct addrinfo *server;
  struct addrinfo *serverRessource;

  int result = -1;
  ///
  if((result = getaddrinfo(this->mHostName.c_str(), this->mPort.c_str(), &(this->mHints), &(serverRessource))) != 0){
    error("");
    //    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  ///
  for(server = serverRessource; server != NULL; server = server->ai_next){
    ///
    if((this->mSocket = socket(server->ai_family, server->ai_socktype, server->ai_protocol)) == -1){
      error("client: socket");
      continue;
    }

    ///
    if(connect(this->mSocket, server->ai_addr, server->ai_addrlen) == -1) {
      this->closeSocket();
      error("client: connect");
      continue;
    }

    break;
  }

  if(server == NULL) {
    error("");
    //fprintf(stderr, "client: failed to connect\n");
    return -2;
  }

  freeaddrinfo(serverRessource);
  return 0;
}

void Socket::closeSocket(){
  // Close the socket
  if(close(mSocket) == -1){

  }
}

void Socket::error(const char *pMessage){

}

void Socket::run(){
  int received = 0;
  while((received = recv(this->mSocket, this->mBuffer, this->mBufferSize, 0)) > 0){

  }
}


/**
 *
 */
void* Socket::getSocketAddress(struct sockaddr* pSocketAddress){
  if(pSocketAddress->sa_family == AF_INET){
    return &(((struct sockaddr_in*)pSocketAddress)->sin_addr);
  }
  return &(((struct sockaddr_in6*)pSocketAddress)->sin6_addr);
}

