#ifndef __VISION_SERVER_NETWORK_UTILITY_DEFS_H__
#define __VISION_SERVER_NETWORK_UTILITY_DEFS_H__


#define MAX_SOCKET_CONNECTIONS 12

#define CONNECTION_ACCEPT 0x0
#define CONNECTION_DISCONNECT 0x1

/*  unused to be deleted */
#define OPEN_STREAM 0x02
#define END_OF_STREAM 0x03
#define REINITIALIZE_STREAM 0x04
#define STREAM_ID_MESG 0x05
#define VIDEO_FRAME 0x06
#define ROI_FRAME_INFO 0x07
#define KEYPOINTS 0x08
#define KEYPOINT_MATCHES 0x09
#define MODEL_KEYPOINT_EXTRACT 0x0A
#define MODELS_DONE 0x0B
/* end unused */
#define WORK_ACK 0x0C
#define REGISTER_TASK 0x0D
#define REGISTER_TASK_REPLY 0x0E
#define MODEL_CHANGE 0x1D

#define ROI_MATCH 0x0F
#define BOX_REGIONS 0x1E
#define OBJECT 0x19
#define INVALID_PACKET 0xAA
#define SOCKET_DATA_HEADER_SIZE 20

#define HOMOGRAPHY 0x20

typedef struct{
	uint32_t message_type;
	uint32_t stream_id;
	uint32_t message_id;
	uint32_t frame_id;
	uint32_t message_length;
	uint8_t * data;
} socket_data;
#endif
