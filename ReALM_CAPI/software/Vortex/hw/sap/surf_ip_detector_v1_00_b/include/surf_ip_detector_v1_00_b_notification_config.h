#ifndef __SURF_IP_DETECTOR_NOTIFICATION_CONFIG_H__
#define __SURF_IP_DETECTOR_NOTIFICATION_CONFIG_H__

#include <VortexTypes.h>

class surf_ip_detector_v1_00_b_notification_config : public IVortexConfiguration
{
public:
	surf_ip_detector_v1_00_b_notification_config(void);
	surf_ip_detector_v1_00_b_notification_config(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID);
	~surf_ip_detector_v1_00_b_notification_config(void);

	VortexDeviceAddress* GetTargetDeviceID();
	void SetTargetDeviceID(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID);

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();

private:
	VortexDeviceAddress* m_TargetDeviceID;
	VortexConfigurationPacket* m_Vcp;
};

#endif

