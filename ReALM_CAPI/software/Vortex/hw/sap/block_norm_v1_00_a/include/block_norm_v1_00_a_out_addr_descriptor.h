#ifndef __block_norm_V1_00_A_OUT_ADDR_DESCRIPTOR_H__
#define __block_norm_V1_00_A_OUT_ADDR_DESCRIPTOR_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"


class block_norm_v1_00_a_out_addr_descriptor 
{
 public:
  typedef struct st_block_norm_v1_00_a_out_addr_descriptor{

	// Since there is no place, we do not support norm addr in CAPI - 05/24/2016
	//uint64_t NormAddr;
    //uint16_t NormMemID;

    uint64_t DotpAddr;
    uint16_t DotpMemID;
    uint8_t Bytes[16];

    /*
    uint8_t* GetBytes()
    {
      *((uint64_t*)&Bytes[0]) = (
				 (((uint64_t) NormAddr & 0xFFFFFFFFFul) << 0) |
				 (((uint64_t) NormMemID & 0xFF) << 48));
      *((uint64_t*)&Bytes[8]) = (
				 (((uint64_t) DotpAddr & 0xFFFFFFFFFul) << 0) |
				 (((uint64_t) DotpMemID & 0xFF) << 48));
      return Bytes;
    } 
    */

    // NormAddr and DotpAddr are 64 bit in ReALM_CAPI - ska - 05/24/2016
    // Since there is no place, we do not support norm addr in CAPI - 05/24/2016
    uint8_t* GetBytes()
    {
    	*((uint64_t*)&Bytes[0]) = (
    		(((uint64_t)DotpAddr & 0xFFFFFFFFFFFFFFFFul) << 0));

    	*((uint64_t*)&Bytes[8]) = (
    		(((uint64_t)DotpMemID & 0xFF)));
    	return Bytes;
    }

  } PayloadType;
public:
	block_norm_v1_00_a_out_addr_descriptor(void);
	~block_norm_v1_00_a_out_addr_descriptor(void);
	
	// Since there is no place, we do not support norm addr in CAPI - 05/24/2016
	/*
	uint64_t GetNormAddr();
	void SetNormAddr(uint64_t value);
	uint16_t GetNormMemID();
	void SetNormMemID(uint16_t value);
	*/
	uint64_t GetDotpAddr();
	void SetDotpAddr(uint64_t value);
	uint16_t GetDotpMemID();
	void SetDotpMemID(uint16_t value);
	
	uint8_t* GetBytes();
private:
	PayloadType m_payload;
};

#endif
