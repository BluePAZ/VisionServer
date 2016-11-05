#ifndef __AIM_OPCODE_CONFIG__
#define __AIM_OPCODE_CONFIG__

#include <VortexTypes.h>

enum class AIMMode { INDEX = 0, BUILD = 1};

class AIMOpcodeConfig : public IVortexConfiguration
{
public:
	AIMOpcodeConfig(AIMMode Mode, uint8_t Iteration, uint8_t ChannelSelect, int LinkControl, bool IsFirst = false);
	~AIMOpcodeConfig(void);

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