#ifndef __HOG_ENGINE_V1_00_A_HISTOGRAM_PAGETABLE_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_HISTOGRAM_PAGETABLE_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _HistogramPageTableConfig {
	
	//@ I am not using opcode in the current version of the hardware - ska - 10/20/2015
	// Histogram PageTable configurations: 
	// -------------------------------------------------------------------------
	// |  RESERVED										       |   GROUP ID    |
	// -------------------------------------------------------------------------
	//  127                                                  32|31            0|

	vector<uint32_t> m_GroupID;

	// Each histogram cell will have one such configuration, so a vector of bytes
	vector<uint8_t> Bytes;

	vector<uint8_t> GetBytes()
	{
		for (int i = 0; i < m_GroupID.size(); i++)
		{
			// Lower most byte
			Bytes.push_back((uint32_t)(m_GroupID[i]) & 0x000000FF); // First byte of 32 bit gid
			Bytes.push_back(((uint32_t)(m_GroupID[i]) & 0x0000FF00) >> 8); // Second byte of 32 bit gid
			Bytes.push_back(((uint32_t)(m_GroupID[i]) & 0x00FF0000) >> 16); // Third byte of 32 bit gid
			Bytes.push_back(((uint32_t)(m_GroupID[i]) & 0xFF000000) >> 24); // Fourth byte of 32 bit gid
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			Bytes.push_back(0); // 1 byte space
			// Upper most byte
			
		}

		return Bytes;
	}



} HistogramPageTableConfig;

class hog_engine_v1_00_a_histogram_pagetable_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_histogram_pagetable_config(uint32_t* GroupID);
	hog_engine_v1_00_a_histogram_pagetable_config(void);
	~hog_engine_v1_00_a_histogram_pagetable_config(void);

	//GroupIDs
	vector<uint32_t> GetGroupIDs();
	void SetGroupIDs(vector<uint32_t> values);
	vector<uint32_t> CreateHOGHistogramPartition(uint16_t MaxImageWidth, uint16_t MaxImageHeight, uint16_t CellWidth, uint16_t CellHeight);

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
	HistogramPageTableConfig m_HistogramPageTableConfig;
};

#endif // !1

