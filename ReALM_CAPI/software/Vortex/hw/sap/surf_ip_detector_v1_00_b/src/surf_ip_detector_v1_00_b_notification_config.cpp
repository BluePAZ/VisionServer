#include "surf_ip_detector_v1_00_b_notification_config.h"

surf_ip_detector_v1_00_b_notification_config::surf_ip_detector_v1_00_b_notification_config(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

surf_ip_detector_v1_00_b_notification_config::surf_ip_detector_v1_00_b_notification_config(void)
{
	m_TargetDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

surf_ip_detector_v1_00_b_notification_config::~surf_ip_detector_v1_00_b_notification_config(void)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);
}

VortexDeviceAddress* surf_ip_detector_v1_00_b_notification_config::GetTargetDeviceID()
{
	return m_TargetDeviceID;
}

void surf_ip_detector_v1_00_b_notification_config::SetTargetDeviceID(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);

	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

VortexEntityType surf_ip_detector_v1_00_b_notification_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void surf_ip_detector_v1_00_b_notification_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t surf_ip_detector_v1_00_b_notification_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void surf_ip_detector_v1_00_b_notification_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* surf_ip_detector_v1_00_b_notification_config::GetConfiguration()
{
	return m_Vcp;
}
