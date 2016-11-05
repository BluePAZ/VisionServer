#ifndef __CAPI_NETWORK_UTILITY_H__
#define __CAPI_NETWORK_UTILITY_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>

#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#include "../capi_network_defs.h"


socket_data receive_data_packet(int socket_fd, int * err);
uint8_t * receiveLength(int socket_fd, int length);
void send_data_packet(int socket_fd, socket_data data);
void decode_data_packet(socket_data data);
void print_data_packet(socket_data * header);

socket_data build_data_packet(uint32_t message_type, uint32_t stream_id, uint32_t message_id, uint32_t frame_id, uint32_t message_length, uint8_t * data);

#endif
