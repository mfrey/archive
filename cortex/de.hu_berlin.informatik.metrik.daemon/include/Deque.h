#ifndef _DEQUE_H_ 
#define _DEQUE_H_

#include <deque>
#include <boost/thread.hpp>

namespace de {
  namespace hu_berlin {
    namespace informatik {
      namespace metrik {
        namespace daemon {
          template <typename Data>
          class Deque {
            private: 
              /// The deque which holds data
              std::deque<Data> mData;
              /// A mutex for controlling several operations on the deque
              mutable boost::mutex mMutex;
              /// A condition variable which allows to notify a thread if the status of the deque has changed
              boost::condition_variable mStatus;

            public:
              /// The method adds an element at the front of the deque
              void pushFront(Data const& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);
                this->mData.push_front(pValue);
                lock.unlock();
                this->mStatus.notify_one();
              }

              /// The method adds an element at the back of the deque
              void pushBack(Data const& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);
                this->mData.push_back(pValue);
                lock.unlock();
                this->mStatus.notify_one();
              }

              /// The method checks if the deque contains no element
              bool empty() const {
                boost::mutex::scoped_lock lock(this->mMutex);
                return this->mData.empty();
              }
               
              /// The method removes an element at the front of the deque
              bool tryPopFront(Data& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);
                if(this->mData.empty()){
                  return false;
                }
        
                pValue = this->mData.front();
                this->mData.pop_front();
                return true;
              }

              /// The method removes an element at the back of the deque
              bool tryPopBack(Data& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);
                if(this->mData.empty()){
                  return false;
                }
        
                pValue = this->mData.back();
                this->mData.pop_back();
                return true;
              }

              /// The method returns the size of the deque
              int size(){
                boost::mutex::scoped_lock lock(this->mMutex);
                return this->mData.size();
              }

              /// The method returns the first element of the deque
              void front(Data& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);
                pValue = this->mData.front();
              }

              /// The method returns the last element of the deque
              void back(Data& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);
                pValue = this->mData.back();
              }

              /// The method clears the content of the deque
              void clear(){
                boost::mutex::scoped_lock lock(this->mMutex);
                this->mData.clear();
              }

              /// The method
	      void waitAndPopFront(Data& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);

                while(this->mData.empty()){
                  this->mStatus.wait(lock);
                }
        
                pValue = this->mData.front();
                this->mData.pop_front();
              }

              /// The method
              void waitAndPopBack(Data& pValue){
                boost::mutex::scoped_lock lock(this->mMutex);

                while(this->mData.empty()){
                  this->mStatus.wait(lock);
               }  
        
               pValue = this->mData.back();
               this->mData.pop_back();
             }
          };
        }
      }
    }
  }
}

#endif
