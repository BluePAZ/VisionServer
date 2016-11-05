#ifndef __VISION_SERVER_STREAM_CLIENT_H__
#define __VISION_SERVER_STREAM_CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "VisionServerNetworkUtility/VisionServerNetworkUtility.hpp"
#include "SocketData/SocketData.hpp"

class VisionServerStreamClient{
public:
        VisionServerStreamClient();
        ~VisionServerStreamClient();

        int connect(const char * ipaddress, int port);
        void disconnect();

        SocketData * receivePacket();
        void sendPacket(SocketData * data);
private:
        CAPINetworkUtility * network;
};
#endif
