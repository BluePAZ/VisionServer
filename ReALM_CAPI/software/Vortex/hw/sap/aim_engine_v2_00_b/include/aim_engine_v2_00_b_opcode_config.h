#ifndef __AIM_ENGINE_V2_00_B_OPCODE_CONFIG__
#define __AIM_ENGINE_V2_00_B_OPCODE_CONFIG__

#include <VortexTypes.h>

enum class AIMMode { INDEX = 0, BUILD = 1};

class aim_opcode_config : public IVortexConfiguration
{
public:
	aim_opcode_config(AIMMode Mode, uint8_t Iteration, uint8_t ChannelSelect, int LinkControl, bool IsFirst = false);
	~aim_opcode_config(void);

	uint16_t GetOpcode();
	int GetLinkControl();

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();
private:
	uint16_t m_Opcode;
	int m_LinkControl;
	VortexConfigurationPacket* m_Vcp;
};

#endif
