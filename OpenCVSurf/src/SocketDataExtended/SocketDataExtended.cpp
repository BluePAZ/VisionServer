#include "SocketDataExtended.hpp"

SocketDataExtended::SocketDataExtended(){
	
}
SocketDataExtended::~SocketDataExtended(){
	if(data != NULL){
		free(data);
		data = NULL;
	}
}
ROIRegion SocketDataExtended::extractROIRegion(uint8_t * data, uint32_t length, uint32_t offset){
	ROIRegion points;
	uint8_t * tmpData = &(data[offset]);
    points.x0 = (uint32_t)((((((tmpData[3] << 8) | tmpData[2]) << 8) | tmpData[1]) << 8) | tmpData[0]);
	points.y0 = (uint32_t)((((((tmpData[7] << 8) | tmpData[6]) << 8) | tmpData[5]) << 8) | tmpData[4]);
	points.x1 = (uint32_t)((((((tmpData[11] << 8) | tmpData[10]) << 8) | tmpData[9]) << 8) | tmpData[8]);
	points.y1 = (uint32_t)((((((tmpData[15] << 8) | tmpData[14]) << 8) | tmpData[13]) << 8) | tmpData[12]);
	points.x2 = (uint32_t)((((((tmpData[19] << 8) | tmpData[18]) << 8) | tmpData[17]) << 8) | tmpData[16]);
	points.y2 = (uint32_t)((((((tmpData[23] << 8) | tmpData[22]) << 8) | tmpData[21]) << 8) | tmpData[20]);
	points.x3 = (uint32_t)((((((tmpData[27] << 8) | tmpData[26]) << 8) | tmpData[25]) << 8) | tmpData[24]);
	points.y3 = (uint32_t)((((((tmpData[31] << 8) | tmpData[30]) << 8) | tmpData[29]) << 8) | tmpData[28]);
	return points;
}
SocketDataExtended::SocketDataExtended(uint8_t * in_data, uint32_t length){
	if(in_data != NULL && length >= SocketDataExtended::getExtendedHeaderSize())
	{
		mode = (uint32_t)((((((in_data[3] << 8) | in_data[2]) << 8) | in_data[1]) << 8) | in_data[0]);
		model_id = (uint32_t)((((((in_data[7] << 8) | in_data[6]) << 8) | in_data[5]) << 8) | in_data[4]);
		//message_length = (uint32_t)((((((data[19] << 8) | data[18]) << 8) | data[17]) << 8) | data[16]);
		region = SocketDataExtended::extractROIRegion(in_data, length, 8);
		int copy_len = length - SocketDataExtended::getExtendedHeaderSize();
		data = (uint8_t *)malloc(copy_len);
		memcpy(data, in_data, copy_len);
		//if(message_length == (length - SocketData::getHeaderSize())){
		//	this->data = (uint8_t*)malloc(message_length);
		//	memcpy(this->data, &data[SocketData::getHeaderSize()], message_length);
		//}else{
		//	this->data = NULL;
		//	message_length = 0;
		//}
	}
}
/*
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
*/
/*
void SocketData::printPacket(){
	fprintf(stdout, "/****Socket Header Print***\n");
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
	fprintf(stdout, "/***************************\n");
}
*/
