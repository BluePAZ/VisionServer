#include "aim_engine_v2_00_b_notification_config.h"

aim_notification_config::aim_notification_config(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

aim_notification_config::aim_notification_config(void)
{
	m_TargetDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

aim_notification_config::~aim_notification_config(void)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);
}

VortexDeviceAddress* aim_notification_config::GetTargetDeviceID()
{
	return m_TargetDeviceID;
}

void aim_notification_config::SetTargetDeviceID(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID)
{
	delete(m_TargetDeviceID);
	delete(m_Vcp);

	m_TargetDeviceID = new VortexDeviceAddress(TargetBusID,TargetSwitchID,TargetPortID);
	m_Vcp = new VortexConfigurationPacket(m_TargetDeviceID->GetBytes(),2,0x00000);
}

VortexEntityType aim_notification_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void aim_notification_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t aim_notification_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void aim_notification_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* aim_notification_config::GetConfiguration()
{
	return m_Vcp;
}
