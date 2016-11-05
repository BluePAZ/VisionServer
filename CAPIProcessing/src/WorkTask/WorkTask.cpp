/*
 * WorkTask.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#include "WorkTask.h"

WorkTask::WorkTask() {
	// TODO Auto-generated constructor stub
	isSingleThreaded = false;
}
WorkTask::~WorkTask(){
	if(network != NULL){
		delete network;
		network = NULL;
	}
}
void WorkTask::registerSocket(int socket, int stream_id){
	socket_stream_id = stream_id;//may be unsed
	network = new CAPINetworkUtility(socket);
	if(network != NULL){
		fprintf(stderr, "socket registered!\n");
	}else{
		fprintf(stderr, "socket not registered!\n");
	}
}
void WorkTask::startTask(){
	fprintf(stderr, "starting task!\n");
	init();
	running = 1;
	int err = StartInternalThread();
	if(err == 0){
	}else{
		fprintf(stderr, "error starting task errno = %i !\n", err);
		fprintf(stderr,"%s\n", strerror(err));
		stopTask();
	}
}
void WorkTask::stopTask(){
	running = 0;
	while(busy != 0){
		//wait for done with current
	}
	deInit(); //call to deinit to shutdown & cleanup
	if(network != NULL){
		delete network;
		network = NULL;
	}
}
void WorkTask::packetReceived(SocketData * data){
	runTask(data);
}
void WorkTask::InternalThreadEntry(){
	network->sendDataPacket(new SocketData(REGISTER_TASK_REPLY,0,0,0,0,NULL));
	fprintf(stderr, "SEND TASK REPLY\n");
	while(running){  //signal to end work
		if(network != NULL){
			int err = 0;
			SocketData * data = network->receiveDataPacket(&err);
			if(err == 1){
				fprintf(stderr, "Error\n");

				stopTask();
			}else{
				packetReceived(data);
				delete data;
			}
		}
	}
	busy = 0;
	pthread_exit(0);
}
void WorkTask::sendDataPacket(SocketData * data){
	if(network != NULL){
		network->sendDataPacket(data);
	}
}
