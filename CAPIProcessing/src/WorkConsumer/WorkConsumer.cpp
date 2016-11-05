#include "WorkConsumer.hpp"

WorkConsumer::WorkConsumer(){
	workStatusChanged = NULL;
	inputQueue = NULL;
	busy = 0;
	running = 0;
}
WorkConsumer::~WorkConsumer(){
	
}
void WorkConsumer::workComplete(){
	if(workStatusChanged != NULL){
		workStatusChanged(workerId, WORK_COMPLETED);
	}
}
void WorkConsumer::setWorkerID(uint32_t id){
	workerId = id;
}
uint8_t WorkConsumer::isHardware(){
	return hardwareAcclerator;
}
void WorkConsumer::startWork(){
	init();
	if(workStatusChanged != NULL){
		workStatusChanged(workerId, WORK_STARTED);
	}
	busy = 1;
	int err = StartInternalThread();
	if(err != 0){
		running = 1;
		if(workStatusChanged != NULL){
			workStatusChanged(workerId, WORK_STOP_ERROR);
		}
		busy = 0;
	}else{

	}
}
void WorkConsumer::stopWork(){
	running = 0; //signal to end after current job
	while(busy != 0){
		;//spin and wait for accelerator to finish current job;
	}
	deInit(); //call to deinit to shutdown & cleanup
	workComplete();
}
void WorkConsumer::InternalThreadEntry(){
	if(inputQueue != NULL){
		while(running){  //signal to end work 
			WorkItem * work = inputQueue->getWork();
			if(work != NULL){
				runWork(work);
				work->completed = 1;
			}
		}
	}
	busy = 0;
	pthread_exit(0);
}

