#include "include/Deque.h"

using namespace de::hu_berlin::informatik::metrik::daemon;

template<typename Data> inline bool Deque<Data>::tryPopFront(Data& pValue){
  boost::mutex::scoped_lock lock(this->mMutex);
  if(this->mData.empty()){
    return false;
  }
        
  pValue = this->mData.front();
  this->mData.pop_front();
  return true;
}

template<typename Data> inline bool Deque<Data>::tryPopBack(Data& pValue){
  boost::mutex::scoped_lock lock(this->mMutex);
  if(this->mData.empty()){
    return false;
  }
        
  pValue = this->mData.back();
  this->mData.pop_back();
  return true;
}

template<typename Data> inline void Deque<Data>::pushFront(Data const& pData){
  boost::mutex::scoped_lock lock(this->mMutex);
  this->mDeque.push_front(pData);
  lock.unlock();
  this->mStatus.notify_one();
}

template<typename Data> inline void Deque<Data>::pushBack(Data const& pData){
  boost::mutex::scoped_lock lock(this->mMutex);
  this->mDeque.push_back(pData);
  lock.unlock();
  this->mStatus.notify_one();
}

template<typename Data> inline bool Deque<Data>::empty(void) const{
  boost::mutex::scoped_lock lock(this->mMutex);
  return this->mData.empty();
}

template<typename Data> inline int Deque<Data>::size(void){
  boost::mutex::scoped_lock lock(this->mMutex);
  return this->mData.size();
}

template<typename Data> inline void Deque<Data>::clear(void){
  boost::mutex::scoped_lock lock(this->mMutex);
  this->mData.clear();
}

template<typename Data> inline void Deque<Data>::waitAndPopFront(Data& pValue){
  boost::mutex::scoped_lock lock(this->mMutex);

  while(this->mData.empty()){
     this->mStatus.wait(lock);
  }
        
  pValue = this->mData.front();
  this->mData.pop_front();
}

template<typename Data> inline void Deque<Data>::waitAndPopBack(Data& pValue){
  boost::mutex::scoped_lock lock(this->mMutex);

  while(this->mData.empty()){
     this->mStatus.wait(lock);
  }
        
  pValue = this->mData.back();
  this->mData.pop_back();
}
