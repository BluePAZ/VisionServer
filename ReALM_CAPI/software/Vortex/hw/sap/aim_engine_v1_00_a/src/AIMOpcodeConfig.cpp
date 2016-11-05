#include "AIMOpcodeConfig.h"

AIMOpcodeConfig::AIMOpcodeConfig(AIMMode Mode, uint8_t Iteration, uint8_t ChannelSelect, int LinkControl, bool IsFirst)
{
	m_LinkControl = LinkControl;

	m_Opcode = (uint16_t)(m_LinkControl & 0x7);
	m_Opcode |= (uint16_t)(Iteration << 3);
    m_Opcode |= (uint16_t)((ChannelSelect & 0x7) << 11);
    m_Opcode |= (uint16_t)((uint16_t)Mode << 14);
    m_Opcode |= (uint16_t)(((IsFirst) ? 1 : 0) << 15);

	m_Vcp = new	VortexConfigurationPacket();
}

AIMOpcodeConfig::~AIMOpcodeConfig(void)
{
	delete(m_Vcp);
}

uint16_t AIMOpcodeConfig::GetOpcode()
{
	return m_Opcode;
}

int AIMOpcodeConfig::GetLinkControl()
{
	return m_LinkControl;
}

VortexEntityType AIMOpcodeConfig::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void AIMOpcodeConfig::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t AIMOpcodeConfig::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void AIMOpcodeConfig::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* AIMOpcodeConfig::GetConfiguration()
{
	uint8_t ConfigBytes[16];

	for (int i=0; i<16; i++)
		ConfigBytes[i] = 0;

	ConfigBytes[0] = ((uint8_t*)&m_Opcode)[0];
	ConfigBytes[1] = ((uint8_t*)&m_Opcode)[1];

	m_Vcp->Clear();
	m_Vcp->Add(ConfigBytes,16);

	return m_Vcp;
}
