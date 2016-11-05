#include "AIMNotificationConfig.h"

AIMNotificationConfig::AIMNotificationConfig(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

AIMNotificationConfig::AIMNotificationConfig(void)
{
	m_TargetDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

AIMNotificationConfig::~AIMNotificationConfig(void)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);
}

VortexDeviceAddress* AIMNotificationConfig::GetTargetDeviceID()
{
	return m_TargetDeviceID;
}

void AIMNotificationConfig::SetTargetDeviceID(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);

	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

VortexEntityType AIMNotificationConfig::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void AIMNotificationConfig::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t AIMNotificationConfig::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void AIMNotificationConfig::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* AIMNotificationConfig::GetConfiguration()
{
	return m_Vcp;
}
