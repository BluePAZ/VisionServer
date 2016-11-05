#ifndef __SOCKET_DATA_HPP__
#define __SOCKET_DATA_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>


#include "../CAPINetworkDefs.hpp"

class SocketData{
	
	public:
		SocketData();
		~SocketData();
		SocketData(uint8_t * data, uint32_t length);
		SocketData(uint32_t message_type, uint32_t stream_id, uint32_t message_id, uint32_t frame_id, uint32_t message_length, uint8_t * data = NULL);
		
		uint32_t message_type;
		uint32_t stream_id;
		uint32_t message_id;
		uint32_t frame_id;
		uint32_t message_length;
		uint8_t * data;
		
		uint8_t * toByteArray();
		void printPacket();
	
		static uint32_t getHeaderSize(){ return 20; }
	private:
		
	
	
};
#endif
