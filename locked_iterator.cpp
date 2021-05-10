#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include "locked_iterator.h"
#include <list>
#include <iostream>

#ifdef PLAIN_MODE
typedef std::list<unsigned int> SampleList;
#elif defined(RECURSIVE_LOCK)
typedef locked_container <
     std::list<unsigned int>,
     boost::recursive_mutex,
     boost::lock_guard<boost::recursive_mutex>
     > SampleList;
#elif defined(EXCLUSIVE_LOCK)
typedef locked_container <
     std::list<unsigned int>,
     boost::mutex,
     boost::lock_guard<boost::mutex>
     > SampleList;
#else
#error Have to choose test mode
#endif

void Internal(SampleList& data)
{
     for (  SampleList::iterator x = data.begin()
          ; x != data.end()
          ; ++x )
     {
          std::cout << '\t' << *x << std::endl;
     }
}

int main()
{
     SampleList data;
     std::cout << "1" << std::endl;
     for (  SampleList::iterator it = data.begin()
          ; it != data.end()
          ; ++it )
     {
          std::cout << *it << std::endl;
     }
     std::cout << "2" << std::endl;
     data.push_back(42);
     std::cout << "3" << std::endl;
     for (  SampleList::iterator it = data.begin()
          ; it != data.end()
          ; ++it )
     {
          std::cout << *it << std::endl;
          Internal(data);
     }
     std::cout << "4" << std::endl;
     return 0;
}
