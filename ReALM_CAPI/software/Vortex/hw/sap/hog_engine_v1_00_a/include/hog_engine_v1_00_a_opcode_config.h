#ifndef __HOG_ENGINE_V1_00_A_OPCODE_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_OPCODE_CONFIG_H__

#include <VortexTypes.h>

class hog_engine_v1_00_a_opcode_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_opcode_config(void);
	hog_engine_v1_00_a_opcode_config(bool Mode, bool IsFirst);
	~hog_engine_v1_00_a_opcode_config(void);

	uint16_t GetOpcode();
	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();

private:
	uint16_t m_Opcode;
	VortexConfigurationPacket* m_Vcp;
};

#endif	