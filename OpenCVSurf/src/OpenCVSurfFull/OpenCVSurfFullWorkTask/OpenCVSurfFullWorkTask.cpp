/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfFullWorkTask.h"

OpenCVSurfFullWorkTask::OpenCVSurfFullWorkTask() {
	// TODO Auto-generated constructor stub
}
OpenCVSurfFullWorkTask::~OpenCVSurfFullWorkTask() {
	// TODO Auto-generated destructor stub
}

void OpenCVSurfFullWorkTask::init(){
	fprintf(stderr, "OpenCVSurfFullWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void OpenCVSurfFullWorkTask::deInit(){
    	fprintf(stderr, "OpenCVSurfFullWorkTask called deInit()\n");
	//TODO: implement cleanup
}
void OpenCVSurfFullWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}


/*
void OpenCVSurfFullWorkTask::packetReceived(SocketData * data){
	runTask(data);
}
*/
std::string OpenCVSurfFullWorkTask::returnTaskName(){
	return "opencv_surf_full";
}
std::string OpenCVSurfFullWorkTask::getTaskName(){
	return "opencv_surf_full";
}

void OpenCVSurfFullWorkTask::setParams(WorkItem * i, uint32_t width, uint32_t height, uint32_t depth){
	i->params = (uint8_t *)malloc(12);
	i->params[3] = width >> 24;
	i->params[2] = width >> 16;
	i->params[1] = width >> 8;
	i->params[0] = width;
	//set height

	i->params[7] = height >> 24;
	i->params[6] = height >> 16;
	i->params[5] = height >> 8;
	i->params[4] = height;

	i->params[11] = depth >> 24;
	i->params[10] = depth >> 16;
	i->params[9] = depth >> 8;
	i->params[8] = depth;

	i->params_length = 12;
}


void OpenCVSurfFullWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "OpenCVSurfFullWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
	uint32_t width = data_in->stream_id;
	uint32_t height = data_in->message_id;
	uint32_t depth = 4;
	uint32_t params[3] = {width, height, depth};
    i->uuid = data_in->frame_id;
	i->setParameters(params, (uint32_t)3);
	i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "OpenCVSurfFullWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}
//actual work flow to run;
void OpenCVSurfFullWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "OpenCVSurfFullWorkTask running\n");

	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_surf_ip", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
		arg->caller->dispatcher->submitWork("opencv_surf_descriptor", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
		//process to send to matcher
		arg->caller->dispatcher->submitWork("opencv_surf_bffmatcher", i);
		while(i->completed == 0){
				//spin and wait for work to be done
		}
		SocketData * returnData = new SocketData(ROI_MATCH, 0,0,i->uuid,i->result_length,(uint8_t *) i->result);
		fprintf(stderr,"length field of return is %lu\n", returnData->message_length);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVSurfFullWorkTask the caller no longer exists!");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}


