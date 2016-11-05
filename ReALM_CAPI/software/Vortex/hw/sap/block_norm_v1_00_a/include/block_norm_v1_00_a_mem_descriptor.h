#ifndef __block_norm_V1_00_A_MEM_CONFIG_H__
#define __block_norm_V1_00_A_MEM_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"


class block_norm_v1_00_a_mem_descriptor
{
 public:
typedef struct st_block_norm_v1_00_a_mem_descriptor{
  uint64_t SvmAddr;
  uint16_t SvmMemID;

  uint16_t SvmFlowID;
  uint16_t HogFlowID;
  uint16_t DotpFlowID;

  uint8_t Bytes[16];

  // SvmAddr is 64 bit in ReALM_CAPI - ska - 05/24/2016
  /*
  uint8_t* GetBytes()
  {
    *((uint64_t*)&Bytes[0]) = (
			       (((uint64_t) SvmAddr & 0xFFFFFFFFFul) << 0) |
			       (((uint64_t) SvmMemID & 0xFF) << 48));
    *((uint64_t*)&Bytes[8]) = (
			       (((uint64_t) SvmFlowID & 0xFFFF) << 0) |
			       (((uint64_t) HogFlowID & 0xFFFF) << 16) |
			       (((uint64_t) DotpFlowID & 0xFFFF) << 32) |
			       (((uint64_t)          0 & 0xFFFF) << 48));
    return Bytes;
  }
  */

  uint8_t* GetBytes()
    {
  	  *((uint64_t*)&Bytes[0]) = (
  		  (((uint64_t)SvmAddr & 0xFFFFFFFFFFFFFFFFul) << 0));
  	  *((uint64_t*)&Bytes[8]) = (
  		  (((uint64_t)SvmFlowID & 0xFFFF) << 0) |
  		  (((uint64_t)HogFlowID & 0xFFFF) << 16) |
  		  (((uint64_t)DotpFlowID & 0xFFFF) << 32) |
  		  (((uint64_t)SvmMemID & 0xFF) << 48));
  	  return Bytes;
    }

} PayloadType;
 
 public:
  block_norm_v1_00_a_mem_descriptor(void);
  ~block_norm_v1_00_a_mem_descriptor(void);
	
  uint64_t GetSvmAddr();
  void SetSvmAddr(uint64_t value);
  uint16_t GetSvmMemID();
  void SetSvmMemID(uint16_t value);
  
  uint16_t GetSvmFlowID();
  void SetSvmFlowID(uint16_t value);
  uint16_t GetHogFlowID();
  void SetHogFlowID(uint16_t value);
  uint16_t GetDotpFlowID();
  void SetDotpFlowID(uint16_t value);
  
  uint8_t* GetBytes();
 private:
  //  VortexConfigurationPacket* m_Vcp;
  PayloadType m_payload;
};

#endif // !1
