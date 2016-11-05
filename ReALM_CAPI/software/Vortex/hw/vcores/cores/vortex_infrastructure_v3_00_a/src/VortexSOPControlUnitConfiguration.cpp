#include "VortexSOPTypes.h"


VortexSOPControlUnitConfiguration::VortexSOPControlUnitConfiguration(uint64_t BaseAddress, uint16_t VirtualOpcode, uint16_t PhysicalOpcode)
{
	m_Vcp = new VortexConfigurationPacket();
	m_Vcp->SetAddress(BaseAddress);
	m_SOPControllerConfig.PhysicalOpcode = PhysicalOpcode;
	m_SOPControllerConfig.VirtualOpcode = VirtualOpcode;
}

VortexSOPControlUnitConfiguration::VortexSOPControlUnitConfiguration(void)
{
	m_Vcp = new VortexConfigurationPacket();
	m_Vcp->SetAddress(0);
	m_SOPControllerConfig.PhysicalOpcode = 0;
	m_SOPControllerConfig.VirtualOpcode = 0;
}

VortexSOPControlUnitConfiguration::~VortexSOPControlUnitConfiguration(void)
{
	delete(m_Vcp);
}

uint16_t VortexSOPControlUnitConfiguration::GetPhysicalOpcode()
{
	return m_SOPControllerConfig.PhysicalOpcode;
}
	
void VortexSOPControlUnitConfiguration::SetPhysicalOpcode(uint16_t value)
{
	m_SOPControllerConfig.PhysicalOpcode = value;
}

uint16_t VortexSOPControlUnitConfiguration::GetVirtualOpcode()
{
	return m_SOPControllerConfig.VirtualOpcode;
}

void VortexSOPControlUnitConfiguration::SetVirtualOpcode(uint16_t value)
{
	m_SOPControllerConfig.VirtualOpcode = value;
}

uint64_t VortexSOPControlUnitConfiguration::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void VortexSOPControlUnitConfiguration::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexEntityType VortexSOPControlUnitConfiguration::GetConfigurationType()
{
	return VortexEntityType::NONE;
}

void VortexSOPControlUnitConfiguration::SetConfigurationType(VortexEntityType ConfigType)
{
}

VortexConfigurationPacket* VortexSOPControlUnitConfiguration::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_SOPControllerConfig.GetBytes(),16);

	return m_Vcp;
}
