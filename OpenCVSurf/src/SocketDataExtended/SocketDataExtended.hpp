#ifndef __SOCKET_DATA_EXTENDED_HPP__
#define __SOCKET_DATA_EXTENDED_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>

#include "../OpenCVSurfSupport/OpenCVSurfSupport.h"

class SocketDataExtended {
	public:
		SocketDataExtended();
		~SocketDataExtended();
		SocketDataExtended(uint8_t * data, uint32_t length);

		//SocketData(uint32_t message_type, uint32_t stream_id, uint32_t message_id, uint32_t frame_id, uint32_t message_length, uint8_t * data = NULL);
		uint32_t mode;
		uint32_t model_id;
		uint32_t message_length;
		ROIRegion region;
		uint8_t * data;
		//uint8_t * toByteArray();
		//void printPacket();
		static ROIRegion extractROIRegion(uint8_t * data, uint32_t length, uint32_t offset);
		static uint32_t getExtendedHeaderSize(){ return 40; };
};
#endif
