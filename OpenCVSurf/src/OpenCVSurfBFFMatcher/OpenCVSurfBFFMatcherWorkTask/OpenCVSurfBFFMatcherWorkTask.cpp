/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfBFFMatcherWorkTask.h"

OpenCVSurfBFFMatcherWorkTask::OpenCVSurfBFFMatcherWorkTask() {
	// TODO Auto-generated constructor stub

}

OpenCVSurfBFFMatcherWorkTask::~OpenCVSurfBFFMatcherWorkTask() {
	// TODO Auto-generated destructor stub
}

void OpenCVSurfBFFMatcherWorkTask::init(){
	fprintf(stderr, "OpenCVSurfBFFMatcherWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void OpenCVSurfBFFMatcherWorkTask::deInit(){
	fprintf(stderr, "OpenCVSurfBFFMatcherWorkTask called deInit()\n");
	//TODO: implement cleanup
}
void OpenCVSurfBFFMatcherWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}
/*
void OpenCVSurfBFFMatcherWorkTask::packetReceived(SocketData * data){
	//TODO: implement logic to run when a packet is received;
	WorkItem * i = new WorkItem();
	i->completed = 0;
	i->params = NULL;
	i->params_length = 0;
	i->data = data->data;
	i->data_length = data->message_length;
	i->result = NULL;
	i->result_length = 0;
	//submit aim work
	dispatcher->submitWork("surf_ip", i);
	while(i->completed == 0){
		//spin and wait for work to be done
	}
	//TODO:send back results using
	this->sendDataPacket(new SocketData());
	delete i;
}
*/
void OpenCVSurfBFFMatcherWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "OpenCVSurfBFFMatcherWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
	i->completed = 0;
	i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "OpenCVSurfBFFMatcherWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}
//actual work flow to run;
void OpenCVSurfBFFMatcherWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "OpenCVSurfBFFMatcherWorkTask running\n");
	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_surf_bffmatcher", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
        SocketData * returnData = new SocketData(WORK_ACK,0,0,0,NULL);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVSurfBFFMatcherWorkTask the caller has been removed.");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}
std::string OpenCVSurfBFFMatcherWorkTask::returnTaskName(){
	return "opencv_surf_bffmatcher";
}
std::string OpenCVSurfBFFMatcherWorkTask::getTaskName(){
	return "opencv_surf_bffmatcher";
}

