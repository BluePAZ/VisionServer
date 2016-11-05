#include "VortexTypes.h"

VortexGenericConfiguration::VortexGenericConfiguration(VortexConfigurationPacket* ConfigPacket, VortexEntityType ConfigType)
{
	m_Configuration = ConfigPacket;
	m_Index = ConfigPacket->GetAddress();
	m_ConfigurationType = ConfigType;
}

VortexGenericConfiguration::~VortexGenericConfiguration(void)
{
	delete(m_Configuration);
}

VortexEntityType VortexGenericConfiguration::GetConfigurationType()
{
	return m_ConfigurationType;
}

void VortexGenericConfiguration::SetConfigurationType(VortexEntityType ConfigType)
{
	m_ConfigurationType = ConfigType;
}

uint64_t VortexGenericConfiguration::GetIndex()
{
	return m_Index;
}

void VortexGenericConfiguration::SetIndex(uint64_t Index)
{
	m_Index = Index;
}

VortexConfigurationPacket* VortexGenericConfiguration::GetConfiguration()
{
	return m_Configuration;
}

