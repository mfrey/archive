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
              /// A condition variable which allows to notify a thread if the status of the deque has changes
              boost::condition_variable mStatus;

            public:
              /// The method adds an element at the front of the deque
              void pushFront(Data const&);
              /// The method adds an element at the back of the deque
              void pushBack(Data const&);
              /// The method checks if the deque contains no element
              bool empty() const;
              /// The method removes an element at the front of the deque
              bool tryPopFront(Data&);
              /// The method removes an element at the back of the deque
              bool tryPopBack(Data&);
              /// The method returns the size of the deque
              int size();
              /// The method clears the content of the deque
              void clear();
              /// The method
	      void waitAndPopFront(Data&);
              /// The method
              void waitAndPopBack(Data&);
          };
        }
      }
    }
  }
}

#endif
