#ifndef __CAPI_NETWORK_UTILITY_HPP__
#define __CAPI_NETWORK_UTILITY_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <cstring>      // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#include <deque>		  // std::deque
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard
#include <stdexcept>      // std::logic_error
#include <condition_variable>

#ifdef _WIN32
//#include "winsock2.h"
//#include "windows.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#endif

#include "../SocketData/SocketData.hpp"
#include "../CAPINetworkDefs.hpp"

class CAPINetworkUtility;

struct receiveThreadArgs{
	CAPINetworkUtility * self;
	SocketData ** data;
	void (*receiveCallback)(SocketData *);
};

class CAPINetworkUtility{
	public:
		CAPINetworkUtility();
		~CAPINetworkUtility();
#ifdef __WIN32
		CAPINetworkUtility(SOCKET sock);
#endif
#ifdef __linux__
		CAPINetworkUtility(int socket);
#endif
		SocketData * receiveDataPacket(int * err);
		int receiveDataPacketAsync(void (*receiveCallback)(SocketData *));

		void sendDataPacket(SocketData * data);
		void sendDataPacketAsync(SocketData * data);

		int openSocket(const char * ipaddress, uint32_t portnumber);
        void closeSocket();
	private:
        //callbacks for async tasks
        //void (*sendCallback)(void *);
        //SocketData *(receiveCallback)(void *);
        void * receiveDataPacketThread(void *);
        void sendLength(uint8_t * data, uint32_t length);
		uint8_t * receiveLength(int length);
		void send_tcp_packet(uint8_t * data, uint32_t length);

		static void * InternalThreadEntryFunc(void * This) {((struct receiveThreadArgs *)This)->self->receiveDataPacketThread(This); return NULL;}
#ifdef __linux__
		std::mutex mutex_;
        //socket code variables
		pthread_t receiveThread;
        int connected_socket;
        struct sockaddr_in client_addr;      /* our address */
        struct sockaddr_in server_addr;     /* remote address */
        //////////////
        uint8_t * receive_length_linux(int length);
        int open_linux_tcp_socket(const char * ipaddress, uint32_t port_number);
        void close_linux_tcp_socket();
        void send_tcp_packet_linux(uint8_t * data, uint32_t length);
#endif
#ifdef __WIN32
        //Socket Code Variables
        WSADATA            wsaData;
        SOCKET             connectedSocket;
        SOCKADDR_IN        TCPAddr;
        int                iResult;
        int                Port = PORT;
        int                ByteReceived = 5, SelectTiming, ErrorCode;
        char ch = 'Y';
        ////////////////////////
        uint8_t * receive_length_windows(int length);
        void open_windows_tcp_socket(const char * ipaddress, uint32_t port_number);
        void close_windows_tcp_socket();
        void send_tcp_packet_windows(uint8_t * data, uint32_t length);
#endif
};
#endif
