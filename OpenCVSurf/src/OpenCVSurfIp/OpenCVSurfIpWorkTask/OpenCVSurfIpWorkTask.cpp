/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfIpWorkTask.h"

OpenCVSurfIpWorkTask::OpenCVSurfIpWorkTask() {
	// TODO Auto-generated constructor stub

}

OpenCVSurfIpWorkTask::~OpenCVSurfIpWorkTask() {
	// TODO Auto-generated destructor stub
}

void OpenCVSurfIpWorkTask::init(){
	fprintf(stderr, "OpenCVSurfIpWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void OpenCVSurfIpWorkTask::deInit(){
	//TODO: implement cleanup
	fprintf(stderr, "OpenCVSurfIpWorkTask called deInit()\n");
}
void OpenCVSurfIpWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}
/*
void OpenCVSurfIpWorkTask::packetReceived(SocketData * data){
	//TODO: implement logic to run when a packet is received;
    runTask(data);
}
*/
void OpenCVSurfIpWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "OpenCVSurfIpWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
    i->uuid = data_in->frame_id;
	i->completed = 0;
	i->setData(data_in->data, data_in->message_length);
	uint32_t width = data_in->stream_id;
	uint32_t height = data_in->message_id;
	setParams(i, width,height);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "OpenCVSurfIpWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}
//actual work flow to run;
void OpenCVSurfIpWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "OpenCVSurfIpWorkTask running\n");
	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_surf_ip", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
        //TODO:: send data back
		int length = 0;
		SURFKeyPointLite * keyPoints = OpenCVSurfSupport::opencv_to_surfkeypointlite(*((std::vector< cv::KeyPoint > *)i->result));
		length = ((std::vector< cv::KeyPoint > *)i->result)->size();
		delete (std::vector< cv::KeyPoint > *)i->result;
		i->result = NULL;
		i->result = (uint8_t *)OpenCVSurfSupport::keypointslite_to_byte(keyPoints, length, &length);
		i->result_length = length;
        SocketData * returnData = new SocketData(KEYPOINTS,0,0,i->uuid,i->result_length, (uint8_t *)i->result);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVSurfIpWorkTask caller doesn't exist");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}
void OpenCVSurfIpWorkTask::setParams(WorkItem * i, uint32_t width, uint32_t height, uint32_t depth){
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
std::string OpenCVSurfIpWorkTask::returnTaskName(){
	return "opencv_surf_ip";
}
std::string OpenCVSurfIpWorkTask::getTaskName(){
	return "opencv_surf_ip";
}

