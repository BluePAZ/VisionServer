#ifndef __CAPI_NETWORK_UTILITY_C__
#define __CAPI_NETWORK_UTILITY_C__

#include "capi_network_utility.h"

uint8_t * receiveLength(int socket_fd, int length){
	uint8_t * data = (uint8_t *)malloc(length);
	int remaining = length;
	int idx = 0;
	int bytes_received;
	while(remaining > 0){
		bytes_received = recv(socket_fd, &data[idx], remaining, 0);
		if(bytes_received == 0){
			return NULL;
		}
		remaining -= bytes_received;
		idx += bytes_received;
		//fprintf(stderr, "Bytes: %i, Remaining: %i\n", bytes_received, remaining);
	}	
	return data;
}
socket_data receive_data_packet(int socket_fd, int * close){
	socket_data data_packet;
	*close = 0;
	uint8_t * header = receiveLength(socket_fd, SOCKET_DATA_HEADER_SIZE);
	if(header == NULL){
		*close = 1;
		return data_packet;
	}
	//Parse header
	data_packet.message_type = ((((((header[3] << 8) | header[2]) << 8) | header[1]) << 8) | header[0]); 
	data_packet.stream_id = ((((((header[7] << 8) | header[6]) << 8) | header[5]) << 8) | header[4]);
	data_packet.message_id = ((((((header[11] << 8) | header[10]) << 8) | header[9]) << 8) | header[8]);
	data_packet.frame_id = ((((((header[15] << 8) | header[14]) << 8) | header[13]) << 8) | header[12]);
	data_packet.message_length = ((((((header[19] << 8) | header[18]) << 8) | header[17]) << 8) | header[16]);
	free(header);

	if(data_packet.message_length > 0){
		data_packet.data = receiveLength(socket_fd, data_packet.message_length);
		if(data_packet.data == NULL){
			*close = 1;
			return data_packet;
		}
	}
	else{
		data_packet.data = NULL;	
	}
	return data_packet;
}
void send_data_packet(int socket_fd, socket_data data){
	fprintf(stdout, "data.message_length: %i\n", data.message_length);
	int sendLength = SOCKET_DATA_HEADER_SIZE + data.message_length;
	uint8_t * sendData = (uint8_t *)malloc(sendLength);

	sendData[0] = (uint8_t)(data.message_type);
	sendData[1] = (uint8_t)(data.message_type >> 8);
	sendData[2] = (uint8_t)(data.message_type >> 16);
	sendData[3] = (uint8_t)(data.message_type >> 24);

	sendData[4] = (uint8_t)(data.stream_id);
	sendData[5] = (uint8_t)(data.stream_id >> 8);
	sendData[6] = (uint8_t)(data.stream_id >> 16);
	sendData[7] = (uint8_t)(data.stream_id >> 24);

	sendData[8] = (uint8_t)(data.message_id);
	sendData[9] = (uint8_t)(data.message_id >> 8);
	sendData[10] = (uint8_t)(data.message_id >> 16);
	sendData[11] = (uint8_t)(data.message_id >> 24);

	sendData[12] = (uint8_t)(data.frame_id);
	sendData[13] = (uint8_t)(data.frame_id >> 8);
	sendData[14] = (uint8_t)(data.frame_id >> 16);
	sendData[15] = (uint8_t)(data.frame_id >>24);
	
	sendData[16] = (uint8_t)(data.message_length);
	sendData[17] = (uint8_t)(data.message_length >> 8);
	sendData[18] = (uint8_t)(data.message_length >> 16);
	sendData[19] = (uint8_t)(data.message_length >> 24);

	if(data.data != NULL){
		memcpy(&sendData[SOCKET_DATA_HEADER_SIZE], data.data, data.message_length);
		free(data.data);
		data.data = NULL;
	}
	send(socket_fd, (void *)sendData, sendLength, 0);
	free(sendData);
}
void decode_data_packet(socket_data data){
	switch(data.message_type){
		case ROI_FRAME_INFO:{

		}
		case CONNECTION_ACCEPT:{

		}
		case CONNECTION_DISCONNECT:{

		}
		case OPEN_STREAM:{

		}
		case VIDEO_FRAME:{
			
		}
		default:{

			break;
		}
	}
}
socket_data build_data_packet(uint32_t message_type, uint32_t stream_id, uint32_t message_id, uint32_t frame_id, uint32_t message_length, uint8_t * data){
	fprintf(stdout, "/***Building Data Packet****/\n");
	socket_data packet;
	packet.message_type = message_type;
	packet.stream_id = stream_id;
	packet.message_id = message_id;
	packet.frame_id = frame_id;
	packet.message_length = message_length;
	if(message_length > 0){
		packet.data = data;
	}else{
		packet.data = NULL;	
	}
	if(data == NULL){
		packet.message_length = 0;
	}
	return packet;
}
void print_data_packet(socket_data * header){
	fprintf(stdout, "/****Socket Header Print****/\n");
	switch(header->message_type){
		case ROI_FRAME_INFO:{
			fprintf(stdout, "Message Type: ROI_FRAME_INFO\n");
			break;
		}
		case CONNECTION_ACCEPT:{
			fprintf(stdout, "Message Type: CONNECTION_ACCEPT\n");
			break;
		}
		case OPEN_STREAM:{
			fprintf(stdout, "Message Type: OPEN_STREAM\n");
			break;
		}
		case VIDEO_FRAME:{
			fprintf(stdout, "Message Type: VIDEO_FRAME\n");
			break;
		}
		case REINITIALIZE_STREAM:{
			fprintf(stdout, "Message Type: REINITIALIZE_STREAM\n");
			break;
		}
	}
	fprintf(stdout, "Message Message ID: %u\n", header->message_id);
	fprintf(stdout, "Message Stream ID: %u\n", header->stream_id);
	fprintf(stdout, "Message Length (bytes): %u\n", header->message_length);
	fprintf(stdout, "/****************************/\n");
}
#endif

