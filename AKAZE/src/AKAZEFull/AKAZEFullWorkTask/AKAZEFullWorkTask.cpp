/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "AKAZEFullWorkTask.h"

AKAZEFullWorkTask::AKAZEFullWorkTask() {
	// TODO Auto-generated constructor stub

}

AKAZEFullWorkTask::~AKAZEFullWorkTask() {
	// TODO Auto-generated destructor stub
}

void AKAZEFullWorkTask::init(){
	fprintf(stderr, "AKAZEFullWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void AKAZEFullWorkTask::deInit(){
	//TODO: implement cleanup
}
void AKAZEFullWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}

void AKAZEFullWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "AKAZEFullWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
    i->uuid = data_in->frame_id;
	i->completed = 0;
	uint32_t params[3] = {1280, 720, 4};
	i->setParameters(params, 3);
	i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "AKAZEFullWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}
//actual work flow to run;
void AKAZEFullWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "AKAZEFullWorkTask running\n");
	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("akaze_full", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
		//visualize Keypoints;


        //TODO:: send data back
        //SocketData * returnData = new SocketData(KEYPOINTS,0,0,i->uuid, i->result_length, (uint8_t *) i->result);
		//arg->caller->sendDataPacket(returnData);
		//delete returnData;
	}else{
		fprintf(stderr, "AKAZEFullWorkTask caller doesn't exist");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}

std::string AKAZEFullWorkTask::returnTaskName(){
	return "akaze_full";
}
std::string AKAZEFullWorkTask::getTaskName(){
	return "akaze_full";
}

