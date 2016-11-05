#ifndef __CAPI_NETWORK_DEFS_H__
#define __CAPI_NETWORK_DEFS_H__


#define MAX_SOCKET_CONNECTIONS 12

#define CONNECTION_ACCEPT 0x0
#define CONNECTION_DISCONNECT 0x1
#define OPEN_STREAM 0x2
#define END_OF_STREAM 0x3
#define REINITIALIZE_STREAM 0x4
#define STREAM_ID_MESG 0x5
#define VIDEO_FRAME 0x6
#define ROI_FRAME_INFO 0x7
#define KEYPOINTS 0x8
#define KEYPOINT_MATCHES 0x9
#define MODEL_KEYPOINT_EXTRACT 0xA
#define MODELS_DONE 0xB
#define REGISTER_TASK 0xD
#define WORK_ACK 0xC
#define INVALID_PACKET 0xAA

#define SOCKET_DATA_HEADER_SIZE 20


typedef struct{
	uint32_t message_type;
	uint32_t stream_id;//repurpose as width
	uint32_t message_id; //height
	uint32_t frame_id; //split into 4 bytes
	uint32_t message_length;
	uint8_t * data;
} socket_data;
#endif
