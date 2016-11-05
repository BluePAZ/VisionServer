/*
 * CAPISaliencyTask.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#include "CAPITestsWorkTask.h"
CAPITestsWorkTask::CAPITestsWorkTask(){
	// TODO Auto-generated constructor stub
	fprintf(stderr, "CAPITestsWorkTask created\n");
}
CAPITestsWorkTask::~CAPITestsWorkTask(){
	// TODO Auto-generated constructor stub
	fprintf(stderr, "CAPITestsWorkTask destroyed\n");
}
void CAPITestsWorkTask::init(){
	fprintf(stderr, "CAPITestsWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void CAPITestsWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "CAPITestsWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "CAPITestWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}
//actual work flow to run;
void CAPITestsWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "CAPITestsWorkTaskThread running\n");
	WorkItem *i = arg->item;
	if(arg->caller != NULL){
		arg->caller->dispatcher->submitWork("test_work",i);
		while(i->completed == 0){
		}
		SocketData * returnData = new SocketData(WORK_ACK,0,0,0, i->result_length, (uint8_t *)i->result);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "THE CALLER HAS BEEN REMOVED");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}
void CAPITestsWorkTask::deInit(){
	//TODO: implement cleanup
	fprintf(stderr, "CAPITestsWorkTask called deInit()\n");
}
void CAPITestsWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
	fprintf(stderr, "CAPITestsWorkTask called parseParams()\n");
}
/*
void CAPITestsWorkTask::packetReceived(SocketData * data){
	fprintf(stderr, "CAPITestsWorkTask called packetReceived()\n");
	runTask(data);
}
*/
std::string CAPITestsWorkTask::returnTaskName(){
	return "test_task";
}
std::string CAPITestsWorkTask::getTaskName(){
	return "test_task";
}


