#include "CAPIStreamClient.hpp"

CAPIStreamClient::CAPIStreamClient(){
    network = new CAPINetworkUtility();
}
CAPIStreamClient::~CAPIStreamClient(){
	if(network != NULL)
	{
		delete network;
	}
}
int CAPIStreamClient::connect(const char * ipaddress, int port){
    return network->openSocket(ipaddress, port);
}
void CAPIStreamClient::disconnect(){
    network->closeSocket();
}
SocketData * CAPIStreamClient::receivePacket(){
    int err = 0;
    return network->receiveDataPacket(&err);
}
void CAPIStreamClient::sendPacket(SocketData * data){
    network->sendDataPacket(data);
}

