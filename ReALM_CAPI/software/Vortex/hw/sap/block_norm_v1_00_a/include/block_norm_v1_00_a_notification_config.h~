#ifndef __HOG_ENGINE_V1_00_A_NOTIFICATION_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_NOTIFICATION_CONFIG_H__

#include <VortexTypes.h>

class hog_engine_v1_00_a_notification_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_notification_config(void);
	hog_engine_v1_00_a_notification_config(uint8_t TargetBusID, uint8_t TargetSwitchID, uint8_t TargetPortID);
	~hog_engine_v1_00_a_notification_config(void);

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

