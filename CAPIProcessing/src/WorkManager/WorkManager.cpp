#include "WorkManager.hpp"


WorkManager::WorkManager(){
	highestId = 0;
	numWorkers = 0;
	todoWork = new ConcurrentQueue();
}
WorkManager::~WorkManager(){
	delete todoWork;
}

void WorkManager::workItemStatusChanged(uint32_t id, uint8_t new_status){
	switch(new_status){
		case WORK_STARTED:{
			break;
		}
		case WORK_COMPLETED:{
			break;
		}
		case WORK_STOPPED:{
			break;
		}
		case WORK_STOP_ERROR:{
			break;
		}
		default:{
			
		}
	}
}
void WorkManager::enqueueWorker(WorkConsumer * worker){
	uint32_t wid = getNewWorkId();
	worker->workStatusChanged = &workItemStatusChanged;
	worker->inputQueue = todoWork;
	worker->setWorkerID(wid);
	worker->startWork();
	workers.insert(std::make_pair(wid,worker));
	numWorkers++;
}
void WorkManager::dequeueWorker(uint32_t id){
	workers.erase(id);
	returnWorkId(id);
	numWorkers--;
}
uint32_t WorkManager::getNewWorkId(){
	if(!availableIds.empty()){
		uint32_t id = availableIds.back();
		availableIds.pop_back();
		return id;
	}else{
		return ++highestId;
	}
}
void WorkManager::returnWorkId(uint32_t id){
	availableIds.push_back(id);
}
