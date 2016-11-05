#include "CAPINetworkUtility.hpp"

CAPINetworkUtility::CAPINetworkUtility(){

}
CAPINetworkUtility::~CAPINetworkUtility(){
	closeSocket();
}
#ifdef __WIN32
		CAPINetworkUtility::CAPINetworkUtility(SOCKET sock){
			connectedSocket = sock;
		}
#endif
#ifdef __linux__
		CAPINetworkUtility::CAPINetworkUtility(int socket){
			connected_socket = socket;
		}
#endif

SocketData * CAPINetworkUtility::receiveDataPacket(int * close){
	*close = 0;
	uint8_t * header = receiveLength(SocketData::getHeaderSize());
	if(header == NULL){
		*close = 1;
		return NULL;
	}
	SocketData * data_packet = new SocketData(header, SocketData::getHeaderSize());
	free(header);
	if(data_packet->message_length > 0){
		data_packet->data = receiveLength(data_packet->message_length);
		if(data_packet->data == NULL){
			*close = 1;
			return data_packet;
		}
	}
	else{
		data_packet->data = NULL;	
	}
	return data_packet;
}
void * CAPINetworkUtility::receiveDataPacketThread(void * data){
	struct receiveThreadArgs * args = (struct receiveThreadArgs *)data;
	uint8_t * header = receiveLength(SocketData::getHeaderSize());
	*(args->data) = new SocketData(header, SocketData::getHeaderSize());
	if(header == NULL){
		pthread_exit(0);
	}
	(*(args->data)) = new SocketData(header, SocketData::getHeaderSize());
	if((*(args->data))->message_length > 0){
		(*(args->data))->data = receiveLength((*(args->data))->message_length);
		if((*(args->data))->data == NULL){
			//*close = 1;
		}
	}
	else{
		(*(args->data))->data = NULL;
	}
	args->receiveCallback(*(args->data));
	free(args);
	pthread_exit(0);
}
int CAPINetworkUtility::receiveDataPacketAsync(void (*receiveCallback)(SocketData *)){
#ifdef __linux__
	if(receiveCallback != NULL){
		SocketData * data;
		struct receiveThreadArgs * args = (struct receiveThreadArgs *)malloc(sizeof(struct receiveThreadArgs));
		args->self = this;
		args->data = &data;
		args->receiveCallback = receiveCallback;
		pthread_create(&receiveThread, NULL, InternalThreadEntryFunc, (void *)(args));
		return 0;
	}
	return 1;
#endif
#ifdef __WIN32
	//TODO: Implement windows thread functions
    int CAPINetworkUtility::receiveDataPacketAsync(void (*receiveCallback)(SocketData *)){

#endif
}
void CAPINetworkUtility::sendDataPacket(SocketData * data){
	std::unique_lock<std::mutex> mlock(mutex_);
	unsigned char * rawBytes = (unsigned char *)data->toByteArray();
	sendLength( rawBytes, SocketData::getHeaderSize() + data->message_length);
	free(rawBytes);
}
void CAPINetworkUtility::sendDataPacketAsync(SocketData * data){
}
uint8_t * CAPINetworkUtility::receiveLength(int length){
    uint8_t * data = NULL;
    #ifdef _WIN32
        data = receive_length_windows(length);
    #endif
    #ifdef __linux__
        data = receive_length_linux(length);
    #endif
    return data;   
}
void CAPINetworkUtility::sendLength(uint8_t * data, uint32_t length){
    #ifdef _WIN32
        send_tcp_packet_windows(data, length);
    #endif
    #ifdef __linux__
        send_tcp_packet_linux(data, length);
    #endif
}    
int CAPINetworkUtility::openSocket(const char * ipaddress, uint32_t portnumber){
    #ifdef __WIN32
        return open_windows_tcp_socket(ipaddress, portnumber);
    #endif
    #ifdef __linux__
        return open_linux_tcp_socket(ipaddress, portnumber);
    #endif
}
void CAPINetworkUtility::closeSocket(){
    #ifdef __WIN32
        close_windows_tcp_socket();
    #endif
    #ifdef __linux__
        close_linux_tcp_socket();
    #endif
}
void CAPINetworkUtility::send_tcp_packet(uint8_t * data, uint32_t length){
    #ifdef _WIN32
        send_tcp_packet_windows(data, length);
    #endif
    #ifdef __linux__
        send_tcp_packet_linux(data, length);
    #endif
    free(data);
}
#ifdef _WIN32
int CAPINetworkUtility::open_windows_tcp_socket(const char * ipaddress, uint32_t portnumber){
     // Create a new socket to receive datagrams on.
    connectedSocket = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
    if (connectedSocket == INVALID_SOCKET)
    {
        printf("Server: Error at socket(): %ld\n", WSAGetLastError());
        // Clean up
        WSACleanup();
        // Exit with error
        return -1;
    }
    else{
        printf("Server: socket() is OK!\n");
    }
    TCPAddr = {0};
    TCPAddr.sin_family = AF_INET;
    TCPAddr.sin_addr.s_addr = inet_addr(ipaddress);
    TCPAddr.sin_port = htons(portnumber);
    
    iResult = connect(connectedSocket, (SOCKADDR*)&TCPAddr, sizeof(TCPAddr));
    if (iResult == SOCKET_ERROR) {
        closesocket(connectedSocket);
        connectedSocket = INVALID_SOCKET;
        return -1;
    }
    return 0;
}
void CAPINetworkUtility::close_windows_tcp_socket(){
    closesocket(connectedSocket);
}
void CAPINetworkUtility::send_tcp_packet_windows(uint8_t * data, uint32_t length){
    iResult = send( connectedSocket, data, (int)length, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connectedSocket);
        WSACleanup();
        return 1;
    }
}
uint8_t * CAPINetworkUtility::receive_length_windows(int length){
    	uint8_t * data = (uint8_t *)malloc(length);
	int remaining = length;
	int idx = 0;
	int bytes_received;
	while(remaining > 0){
		bytes_received = recv(connectedSocket, &data[idx], remaining, 0);
		if(bytes_received <= 0){
			return NULL;
		}
		remaining -= bytes_received;
		idx += bytes_received;
	}	
	return data;
}
#endif

#ifdef __linux__
uint8_t * CAPINetworkUtility::receive_length_linux(int length){
    uint8_t * data = (uint8_t *)malloc(length);
	int remaining = length;
	int idx = 0;
	int bytes_received;
	while(remaining > 0){
		bytes_received = recv(connected_socket, &data[idx], remaining, 0);
		if(bytes_received == 0){
			return NULL;
		}
		remaining -= bytes_received;
		idx += bytes_received;
	}	
	return data;
}
int CAPINetworkUtility::open_linux_tcp_socket(const char * ipaddress, uint32_t port_number){
    //Create socket
    connected_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (connected_socket == -1)
    {
        fprintf(stdout, "Unable to create socket\n\r");
        return 1;
    }
    server_addr.sin_addr.s_addr = inet_addr(ipaddress);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( port_number );
    //Connect to remote server
    if (connect(connected_socket , (struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
    {
        return 1;
    }
    //connection is now open
    return 0;
}
void CAPINetworkUtility::close_linux_tcp_socket(){
    close(connected_socket);
}
void CAPINetworkUtility::send_tcp_packet_linux(uint8_t * data, uint32_t length){
	int err = send(connected_socket, (void *)data, length, MSG_NOSIGNAL);
	if(err < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}
}
#endif
