#ifndef __HOG_ENGINE_V1_00_A_CONVOLUTION_IMAGE_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_CONVOLUTION_IMAGE_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _ConvolutionImageConfig {
	
	//@ I am not using opcode in the current version of the hardware - ska - 10/19/2015
	// Convolution Image configurations:
    // ----------------------------------------------------------------------------------------
    // |  RESERVED        | KERNEL HEIGHT    | KERNEL WIDTH    | IMG HEIGHT    | IMG WIDTH     |
    // ----------------------------------------------------------------------------------------
    //  127             48|47              40|39             32|31           16|15             0

    uint16_t Width          ;
    uint16_t Height         ;
	uint8_t KernelWidth     ;
	uint8_t KernelHeight    ;    
	
	uint8_t Bytes[16];

    uint8_t* GetBytes()
    {
        *((uint64_t*)&Bytes[0]) =   (((uint64_t)Width			& 0xFFFF) << 0 ) |
									(((uint64_t)Height          & 0xFFFF) << 16) |
                                    (((uint64_t)KernelWidth		& 0x00FF) << 32) |
                                    (((uint64_t)KernelHeight	& 0x00FF) << 40) ;

        *((uint64_t*)&Bytes[8]) =   0;

        return Bytes;
    }
} ConvolutionImageConfig;

class hog_engine_v1_00_a_convolution_image_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_convolution_image_config(uint16_t Width, uint16_t Height);
	hog_engine_v1_00_a_convolution_image_config(void);
	~hog_engine_v1_00_a_convolution_image_config(void);

	//Width
	uint16_t GetWidth();
	void SetWidth(uint16_t value);

	//Height
	uint16_t GetHeight();
	void SetHeight(uint16_t value);

	//Kernel Width
	uint8_t GetKernelWidth();
	void SetKernelWidth(uint8_t value);

	//Kernel Height
	uint8_t GetKernelHeight();
	void SetKernelHeight(uint8_t value);

	//Opcode
	//uint8_t GetOpcode();
	//void SetOpcode(uint8_t value);

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();

private:
	VortexConfigurationPacket* m_Vcp;
	ConvolutionImageConfig m_ConvolutionImageConfig;
};

#endif // !1

