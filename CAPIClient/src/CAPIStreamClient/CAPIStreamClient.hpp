#ifndef __CAPI_STREAM_CLIENT_H__
#define __CAPI_STREAM_CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CAPINetworkUtility/CAPINetworkUtility.hpp"
#include "SocketData/SocketData.hpp"

class CAPIStreamClient{
public:
        CAPIStreamClient();
        ~CAPIStreamClient();

        int connect(const char * ipaddress, int port);
        void disconnect();

        SocketData * receivePacket();
        void sendPacket(SocketData * data);
private:
        CAPINetworkUtility * network;
};
#endif
