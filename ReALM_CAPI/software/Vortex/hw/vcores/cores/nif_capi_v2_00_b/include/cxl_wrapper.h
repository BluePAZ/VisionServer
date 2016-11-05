#ifndef CXL_WRAPPER_H_INCLUDED
#define CXL_WRAPPER_H_INCLUDED

#include <stdint.h>

typedef struct  _HostMessageReceiveStatus{
    bool MessageInterruptPending;
    bool MessageAvailable;
    uint16_t MessageSizeBytes;
}HostMessageReceiveStatus;

typedef struct  _HostMessageSendStatus{
    bool MessageSlotAvailable;
}HostMessageSendStatus;

int cxl_initialize();
void cxl_deinitialize();
void mmio_write64(uint64_t offset, uint64_t data);
void mmio_write32(uint64_t offset, uint32_t data);
uint64_t mmio_read64(uint64_t offset);
uint32_t mmio_read32(uint64_t offset);

HostMessageReceiveStatus GetHostMessageReceiveQueueStatus();
HostMessageSendStatus GetHostMessageSendQueueStatus();
void HostMessageSendBytes(uint8_t* data, int count);
uint8_t* HostMessageRecvBytes(int count);

#endif // CXL_WRAPPER_H_INCLUDED
