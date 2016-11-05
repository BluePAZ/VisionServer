#include "aim_engine_v2_00_b_opcode_config.h"

aim_opcode_config::aim_opcode_config(AIMMode Mode, uint8_t Iteration, uint8_t ChannelSelect, int LinkControl, bool IsFirst)
{
	m_LinkControl = LinkControl;

	m_Opcode = (uint16_t)(m_LinkControl & 0x7);
	m_Opcode |= (uint16_t)(Iteration << 3);
    m_Opcode |= (uint16_t)((ChannelSelect & 0x7) << 11);
    m_Opcode |= (uint16_t)((uint16_t)Mode << 14);
    m_Opcode |= (uint16_t)(((IsFirst) ? 1 : 0) << 15);

	m_Vcp = new	VortexConfigurationPacket();
}

aim_opcode_config::~aim_opcode_config(void)
{
	delete(m_Vcp);
}

uint16_t aim_opcode_config::GetOpcode()
{
	return m_Opcode;
}

int aim_opcode_config::GetLinkControl()
{
	return m_LinkControl;
}

VortexEntityType aim_opcode_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void aim_opcode_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t aim_opcode_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void aim_opcode_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* aim_opcode_config::GetConfiguration()
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
