#include "hog_engine_v1_00_a_opcode_config.h"

hog_engine_v1_00_a_opcode_config::hog_engine_v1_00_a_opcode_config(bool Mode = true, bool IsFirst = true)
{
	// Opcode configurations:
	// -----------------------------------------------
	// | IS FIRST (1)  | BUILD (1)    |  RESERVED     |
	// -----------------------------------------------
	//      15         |    14        |13             0

	m_Opcode |= (uint16_t)(0x0 << 13);
	m_Opcode |= (uint16_t)((uint16_t)Mode << 14);
	m_Opcode |= (uint16_t)(((IsFirst) ? 1 : 0) << 15);

	m_Vcp = new	VortexConfigurationPacket();
}

hog_engine_v1_00_a_opcode_config::~hog_engine_v1_00_a_opcode_config(void)
{
	delete(m_Vcp);
}

uint16_t hog_engine_v1_00_a_opcode_config::GetOpcode()
{
	return m_Opcode;
}


VortexEntityType hog_engine_v1_00_a_opcode_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_opcode_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t hog_engine_v1_00_a_opcode_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_opcode_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_opcode_config::GetConfiguration()
{
	uint8_t ConfigBytes[16];

	for (int i = 0; i<16; i++)
		ConfigBytes[i] = 0;

	ConfigBytes[0] = ((uint8_t*)&m_Opcode)[0];
	ConfigBytes[1] = ((uint8_t*)&m_Opcode)[1];

	m_Vcp->Clear();
	m_Vcp->Add(ConfigBytes, 16);

	return m_Vcp;
}

