#include "SocketData.hpp"

SocketData::SocketData(){
	
}
SocketData::~SocketData(){
	if(data != NULL){
		free(data);
		data = NULL;
	}
}
SocketData::SocketData(uint8_t * data, uint32_t length){
	if(data != NULL && length >= SocketData::getHeaderSize())
	{
		message_type = (uint32_t)((((((data[3] << 8) | data[2]) << 8) | data[1]) << 8) | data[0]);
		stream_id = (uint32_t)((((((data[7] << 8) | data[6]) << 8) | data[5]) << 8) | data[4]);
		message_id = (uint32_t)((((((data[11] << 8) | data[10]) << 8) | data[9]) << 8) | data[8]);
		frame_id = (uint32_t)((((((data[15] << 8) | data[14]) << 8) | data[13]) << 8) | data[12]);
		message_length = (uint32_t)((((((data[19] << 8) | data[18]) << 8) | data[17]) << 8) | data[16]);
		//if(message_length == (length - SocketData::getHeaderSize())){
		//	this->data = (uint8_t*)malloc(message_length);
		//	memcpy(this->data, &data[SocketData::getHeaderSize()], message_length);
		//}else{
		//	this->data = NULL;
		//	message_length = 0;
		//}
	}
}
SocketData::SocketData(uint32_t message_type, uint32_t stream_id, uint32_t message_id, uint32_t frame_id, uint32_t message_length, uint8_t * data)
{
	this->message_type = message_type;
	this->stream_id = stream_id;
	this->message_id = message_id;
	this->frame_id = frame_id;
	this->data = data;
	if (data != NULL)
	{
		this->data = (uint8_t *)malloc(message_length);
		memcpy(this->data, data, message_length);
		this->message_length = message_length;
	}else{
		this->message_length = 0;
		this->data = NULL;
	}
}
uint8_t * SocketData::toByteArray(){
	uint8_t * data = (uint8_t *)malloc(SocketData::getHeaderSize() + this->message_length);

	uint32_t mt = (uint32_t)message_type;
	data[0] = (uint8_t)mt;
	data[1] = (uint8_t)(mt >> 8);
	data[2] = (uint8_t)(mt >> 16);
	data[3] = (uint8_t)(mt >> 24);

	data[4] = (uint8_t)stream_id; 
	data[5] = (uint8_t)(stream_id >> 8);
	data[6] = (uint8_t)(stream_id >> 16);
	data[7] = (uint8_t)(stream_id >> 24);

	data[8] = (uint8_t)message_id;
	data[9] = (uint8_t)(message_id >> 8);
	data[10] = (uint8_t)(message_id >> 16);
	data[11] = (uint8_t)(message_id >> 24);

	data[12] = (uint8_t)frame_id;
	data[13] = (uint8_t)(frame_id >> 8);
	data[14] = (uint8_t)(frame_id >> 16);
	data[15] = (uint8_t)(frame_id >> 24);

	data[16] = (uint8_t)message_length;
	data[17] = (uint8_t)(message_length >> 8);
	data[18] = (uint8_t)(message_length >> 16);
	data[19] = (uint8_t)(message_length >> 24);

	if ((this->data != NULL) && (this->message_length != 0))
	{   
		memcpy(&data[SocketData::getHeaderSize()], this->data, this->message_length);
	}
	return data;
}
void SocketData::printPacket(){
	fprintf(stdout, "/****Socket Header Print****/\n");
	switch(this->message_type){
		case ROI_FRAME_INFO:
			fprintf(stdout, "Message Type: ROI_FRAME_INFO\n");
			break;
		case CONNECTION_ACCEPT:
			fprintf(stdout, "Message Type: CONNECTION_ACCEPT\n");
			break;
		case OPEN_STREAM:
			fprintf(stdout, "Message Type: OPEN_STREAM\n");
			break;
		case VIDEO_FRAME:
			fprintf(stdout, "Message Type: VIDEO_FRAME\n");
			break;
		case REINITIALIZE_STREAM:
			fprintf(stdout, "Message Type: REINITIALIZE_STREAM\n");
			break;
		case REGISTER_TASK:
			fprintf(stdout, "Message Type: REGISTER_TASK\n");
			break;
	}
	fprintf(stdout, "Message Frame ID: %u\n", this->frame_id);
	fprintf(stdout, "Message Message ID: %u\n", this->message_id);
	fprintf(stdout, "Message Stream ID: %u\n", this->stream_id);
	fprintf(stdout, "Message Length (bytes): %u\n", this->message_length);
	fprintf(stdout, "/****************************/\n");
}
