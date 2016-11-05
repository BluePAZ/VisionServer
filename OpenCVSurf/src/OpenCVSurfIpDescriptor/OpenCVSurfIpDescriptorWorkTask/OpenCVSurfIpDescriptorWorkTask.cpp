/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfIpDescriptorWorkTask.h"

OpenCVSurfIpDescriptorWorkTask::OpenCVSurfIpDescriptorWorkTask() {
	// TODO Auto-generated constructor stub

}

OpenCVSurfIpDescriptorWorkTask::~OpenCVSurfIpDescriptorWorkTask() {
	// TODO Auto-generated destructor stub
}

void OpenCVSurfIpDescriptorWorkTask::init(){
	fprintf(stderr, "OpenCVSurfIpDescriptorWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void OpenCVSurfIpDescriptorWorkTask::deInit(){
	//TODO: implement cleanup
}
void OpenCVSurfIpDescriptorWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}
void OpenCVSurfIpDescriptorWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "OpenCVSurfIpDescriptorWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
    i->uuid = data_in->frame_id;
	i->completed = 0;
	i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "OpenCVSurfIpDescriptorWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}

//actual work flow to run;
void OpenCVSurfIpDescriptorWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "OpenCVSurfIpDescriptorWorkTask running\n");
	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_surf_ip_descriptor", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
        //TODO:: send data back
        SocketData * returnData = new SocketData(KEYPOINTS,0,0,i->uuid, i->result_length, (uint8_t *)i->result);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVSurfIpDescriptorWorkTask caller doesn't exist");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}

std::string OpenCVSurfIpDescriptorWorkTask::returnTaskName(){
	return "opencv_surf_ip_descriptor";
}
std::string OpenCVSurfIpDescriptorWorkTask::getTaskName(){
	return "opencv_surf_ip_descriptor";
}

