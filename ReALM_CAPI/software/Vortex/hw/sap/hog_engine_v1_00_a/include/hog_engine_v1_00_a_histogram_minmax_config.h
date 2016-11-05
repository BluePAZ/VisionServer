#ifndef __HOG_ENGINE_V1_00_A_HISTOGRAM_MINMAX_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_HISTOGRAM_MINMAX_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _HistogramMinMaxConfig {
	
	//@ I am not using opcode in the current version of the hardware - ska - 10/20/2015
	// Histogram MinMax configurations:
	// -------------------------------------------------------------------------
	// |  RESERVED        |       MAX        |        MIN      |   RESERVED    |
	// -------------------------------------------------------------------------
	//  127             96|95              64|63             32|31            0|

    uint32_t Min          ;
    uint32_t Max          ;
    //uint16_t Opcode			;
	
	uint8_t Bytes[16];

	uint8_t* GetBytes()
	{
		// Min 
		*((uint64_t*)&Bytes[0]) = (((uint64_t)0 & 0xFFFFFFFF) << 0) |
								  (((uint64_t)Min & 0xFFFFFFFF) << 32);

		// Max
		*((uint32_t*)&Bytes[8]) = (((uint64_t)Max & 0xFFFFFFFF) << 0) |
								  (((uint64_t)0 & 0xFFFFFFFF) << 32);		

		return Bytes;
	}

} HistogramMinMaxConfig;

class hog_engine_v1_00_a_histogram_minmax_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_histogram_minmax_config(uint32_t Min, uint32_t Max);
	hog_engine_v1_00_a_histogram_minmax_config(void);
	~hog_engine_v1_00_a_histogram_minmax_config(void);

	//Min
	uint16_t GetMin();
	void SetMin(uint32_t value);

	//Max
	uint16_t GetMax();
	void SetMax(uint32_t value);

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
	HistogramMinMaxConfig m_HistogramMinMaxConfig;
};

#endif // !1

