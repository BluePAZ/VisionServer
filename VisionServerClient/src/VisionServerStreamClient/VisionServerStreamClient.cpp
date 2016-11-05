#include "VisionServerStreamClient.hpp"

VisionServerStreamClient::VisionServerStreamClient(){
    network = new CAPINetworkUtility();
}
VisionServerStreamClient::~VisionServerStreamClient(){
	if(network != NULL)
	{
		delete network;
	}
}
int VisionServerStreamClient::connect(const char * ipaddress, int port){
    return network->openSocket(ipaddress, port);
}
void VisionServerStreamClient::disconnect(){
    network->closeSocket();
}
SocketData * VisionServerStreamClient::receivePacket(){
    int err = 0;
    return network->receiveDataPacket(&err);
}
void VisionServerStreamClient::sendPacket(SocketData * data){
    network->sendDataPacket(data);
}

