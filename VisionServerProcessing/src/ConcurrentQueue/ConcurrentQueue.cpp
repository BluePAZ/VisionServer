#include "ConcurrentQueue.hpp"

ConcurrentQueue::ConcurrentQueue(){
	
}
ConcurrentQueue::~ConcurrentQueue(){
	
}
WorkItem * ConcurrentQueue::getWork(){
	std::unique_lock<std::mutex> mlock(mutex_);
	while(workList.empty()){
	    cond_.wait(mlock);
	}
	WorkItem * ret = workList.front();
	workList.pop_front();
	return ret;
}
void ConcurrentQueue::setWork(WorkItem *item){
	std::unique_lock<std::mutex> mlock(mutex_);
	workList.push_back(item);
	mlock.unlock();
	cond_.notify_one();
}
