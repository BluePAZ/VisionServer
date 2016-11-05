#include <errno.h>
#include <stdio.h>

#include "cxl_wrapper.h"
#include "libcxl.h"

#define DEVICE "/dev/cxl/afu0.0d"
#define CACHELINE_BYTES 128
#define AFU_MMIO_REG_SIZE 0x4000000
#define MMIO_TRACE_ADDR   0x3FFFFF8
#define ADDRESS_SHIFT 2

struct cxl_afu_h *afu_h;

int cxl_initialize()
{
    afu_h = cxl_afu_open_dev (DEVICE);
    if (!afu_h) {
      perror("cxl_afu_open_dev:"DEVICE);
      return -1;
    }

    // Send start to AFU
    cxl_afu_attach (afu_h, (__u64) 0xDEADBEEF8BADF00D);

    if ((cxl_mmio_map (afu_h, CXL_MMIO_BIG_ENDIAN)) < 0) {
        perror("cxl_mmio_map:"DEVICE);
        return -1;
      }

    return 0;
}

void cxl_deinitialize()
{
    cxl_afu_free (afu_h);
    afu_h = NULL;
}

void mmio_write64(uint64_t offset, uint64_t data)
{
    cxl_mmio_write64(afu_h,offset << ADDRESS_SHIFT,data);
}

void mmio_write32(uint64_t offset, uint32_t data)
{
    cxl_mmio_write32(afu_h,offset << ADDRESS_SHIFT, data);
}

uint64_t mmio_read64(uint64_t offset)
{   uint64_t data;
    cxl_mmio_read64(afu_h,offset << ADDRESS_SHIFT, &data);
    return data;
}

uint32_t mmio_read32(uint64_t offset)
{
	uint32_t data;
    cxl_mmio_read32(afu_h,offset << ADDRESS_SHIFT, &data);
    return data;
}

HostMessageReceiveStatus GetHostMessageReceiveQueueStatus()
{
    uint64_t statusReg;

    HostMessageReceiveStatus status;
    statusReg = mmio_read64(0x000000);

    status.MessageInterruptPending = (statusReg & 0x8000000000000000);
    status.MessageAvailable = (statusReg & 0x4000000000000000);
    status.MessageSizeBytes = (statusReg & 0x3FF);

    return status;
}

HostMessageSendStatus GetHostMessageSendQueueStatus()
{
    uint64_t statusReg;

    HostMessageSendStatus status;
    statusReg = mmio_read64(0x002000);

    status.MessageSlotAvailable = (statusReg & 0x1);

    return status;
}

void HostMessageSendBytes(uint8_t* data, int count)
{
    for (int i=0; i<count; i+=8){
        //mmio_write64(0x14000,*((uint64_t*)&data[i]));
        mmio_write64(0x002800,*((uint64_t*)&data[i]));
    }
}

uint8_t* HostMessageRecvBytes(int count)
{
    uint8_t* data = new uint8_t[count];

    for (int i=0; i<count; i+=8){
        //*((uint64_t*)&data[i]) = mmio_read64(0x4000);
    	*((uint64_t*)&data[i]) = mmio_read64(0x000800);
    }

    return data;
}
