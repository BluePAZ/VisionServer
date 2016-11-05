/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "../SURFIPWorkTask/SURFIPWorkTask.h"

SURFIPWorkTask::SURFIPWorkTask() {
	// TODO Auto-generated constructor stub

}

SURFIPWorkTask::~SURFIPWorkTask() {
	// TODO Auto-generated destructor stub
}

void SURFIPWorkTask::init(){
	fprintf(stderr, "AIM TASK INIT CALLED!\n");
	//TODO:: implement any startup logic
}
void SURFIPWorkTask::runTask(SocketData * data){
	fprintf(stderr, "AIM TASK RUN TASK CALLED!\n");
}
void SURFIPWorkTask::deInit(){
	//TODO: implement cleanup
}
void SURFIPWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}
/*
void SURFIPWorkTask::packetReceived(SocketData * data){
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
void SURFIPWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "OpenCVSurfIpWorkTask running\n");
	/*
	WorkItem * i = arg->item;
	if(arg->caller != NULL){
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_surf_ip", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
        //TODO:: send data back
        SocketData * returnData = new SocketData(KEYPOINTS,0,0,i->uuid,i->result_length,i->result);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVSurfIpWorkTask caller doesn't exist");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	*/
	pthread_exit(0);
}
std::string SURFIPWorkTask::returnTaskName(){
	return "surf_ip";
}
std::string SURFIPWorkTask::getTaskName(){
	return "surf_ip";
}

