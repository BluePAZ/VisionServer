#ifndef __CONCURRENT_QUEUE_H__
#define __CONCURRENT_QUEUE_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <deque>		  // std::deque
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard
#include <stdexcept>      // std::logic_error
#include <condition_variable>

#include "../WorkItem/WorkItem.hpp"
#include "../WorkTypeDefs.hpp"


class ConcurrentQueue{
public:
	ConcurrentQueue();
	~ConcurrentQueue();
	WorkItem * getWork();
	void setWork(WorkItem * w);
private:
	std::mutex work_lock;
	std::deque<WorkItem *> workList;
	std::mutex mutex_;
	std::condition_variable cond_;
};


#endif
