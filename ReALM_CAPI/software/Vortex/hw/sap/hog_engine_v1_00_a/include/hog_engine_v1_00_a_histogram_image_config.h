#ifndef __HOG_ENGINE_V1_00_A_HISTOGRAM_IMAGE_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_HISTOGRAM_IMAGE_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _HistogramImageConfig {
	
	//@ I am not using opcode in the current version of the hardware - ska - 10/20/2015
	// Histogram Image configurations:
    // -------------------------------------------------------------
    // |   RSVRD  |    HIST         | IMG HEIGHT    | IMG WIDTH     |
    // -------------------------------------------------------------
    //  127     48|47             32|31           16|15             0

    uint16_t Width          ;
    uint16_t Height         ;
    uint16_t NumHistograms	;
	
	uint8_t Bytes[16];

	uint8_t* GetBytes()
	{
		// Lower 64 bit word
		*((uint64_t*)&Bytes[0]) = (((uint64_t)Width & 0xFFFF) << 0) |
								  (((uint64_t)Height & 0xFFFF) << 16) |
								  (((uint64_t)NumHistograms & 0xFFFF) << 32) ;

		// Upper 64 bit word
		*((uint64_t*)&Bytes[8]) = 0;

		return Bytes;
	}

} HistogramImageConfig;

class hog_engine_v1_00_a_histogram_image_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_histogram_image_config(uint16_t Width, uint16_t Height);
	hog_engine_v1_00_a_histogram_image_config(void);
	~hog_engine_v1_00_a_histogram_image_config(void);

	//Width
	uint16_t GetWidth();
	void SetWidth(uint16_t value);

	//Height
	uint16_t GetHeight();
	void SetHeight(uint16_t value);

	//Histograms
	uint16_t GetHistograms();
	void SetHistograms(uint16_t value);

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();

private:
	VortexConfigurationPacket* m_Vcp;
	HistogramImageConfig m_HistogramImageConfig;
};

#endif // !1

