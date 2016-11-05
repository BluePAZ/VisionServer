/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVOclSurfIpDescriptorWorkTask.h"

OpenCVOclSurfIpDescriptorWorkTask::OpenCVOclSurfIpDescriptorWorkTask() {
	// TODO Auto-generated constructor stub

}

OpenCVOclSurfIpDescriptorWorkTask::~OpenCVOclSurfIpDescriptorWorkTask() {
	// TODO Auto-generated destructor stub
}

void OpenCVOclSurfIpDescriptorWorkTask::init(){
	fprintf(stderr, "OpenCVOclSurfIpDescriptorWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void OpenCVOclSurfIpDescriptorWorkTask::deInit(){
	//TODO: implement cleanup
}
void OpenCVOclSurfIpDescriptorWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}
/*
void OpenCVSurfIpDescriptorWorkTask::packetReceived(SocketData * data){
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
	dispatcher->submitWork("surf_ip_descriptor", i);
	while(i->completed == 0){
		//spin and wait for work to be done
	}
	//TODO:send back results using
	//this->sendDataPacket(new SocketData());
	delete i;
}
*/
void OpenCVOclSurfIpDescriptorWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "OpenCVOclSurfIpDescriptorWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
    i->uuid = data_in->frame_id;
	i->completed = 0;
	i->data = (uint8_t *)malloc(data_in->message_length);
	memcpy((void *)i->data, data_in->data, data_in->message_length);
	i->data_length = data_in->message_length;
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "OpenCVOclSurfIpDescriptorWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}

//actual work flow to run;
void OpenCVOclSurfIpDescriptorWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "OpenCVOclSurfIpDescriptorWorkTask running\n");
	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_ocl_surf_ip_descriptor", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
        //TODO:: send data back
        SocketData * returnData = new SocketData(KEYPOINTS,0,0,i->uuid, i->result_length, i->result);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVOclSurfIpDescriptorWorkTask caller doesn't exist");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}

std::string OpenCVOclSurfIpDescriptorWorkTask::returnTaskName(){
	return "opencv_ocl_surf_ip_descriptor";
}
std::string OpenCVOclSurfIpDescriptorWorkTask::getTaskName(){
	return "opencv_ocl_surf_ip_descriptor";
}

