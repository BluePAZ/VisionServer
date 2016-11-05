#include "hog_engine_v1_00_a_notification_config.h"

hog_engine_v1_00_a_notification_config::hog_engine_v1_00_a_notification_config(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

hog_engine_v1_00_a_notification_config::hog_engine_v1_00_a_notification_config(void)
{
	m_TargetDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_notification_config::~hog_engine_v1_00_a_notification_config(void)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);
}

VortexDeviceAddress* hog_engine_v1_00_a_notification_config::GetTargetDeviceID()
{
	return m_TargetDeviceID;
}

void hog_engine_v1_00_a_notification_config::SetTargetDeviceID(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);

	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

VortexEntityType hog_engine_v1_00_a_notification_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_notification_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t hog_engine_v1_00_a_notification_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_notification_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_notification_config::GetConfiguration()
{
	return m_Vcp;
}
