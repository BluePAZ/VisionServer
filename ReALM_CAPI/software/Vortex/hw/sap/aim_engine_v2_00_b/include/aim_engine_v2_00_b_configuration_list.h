#ifndef __AIM_CONFIGURATION_LIST_H__
#define __AIM_CONFIGURATION_LIST_H__

#include "VortexTypes.h"
#include "VortexSAPTypes.h"

class aim_configuration_list
{
public:
	aim_configuration_list(VortexDeviceAddress* DeviceID, uint64_t BaseAddress);
	~aim_configuration_list(void);

	uint64_t GetBaseAddress();
	void SetBaseAddress(uint64_t value);

	VortexDeviceAddress* GetDeviceID();
	void SetDeviceID(VortexDeviceAddress* value);

	vector<VortexConfigurationGroup*> GetConfigurationGroups();
	void Init();
	void Insert(IVortexConfiguration* Entry);
	void Insert(vector<IVortexConfiguration*> Entries);

private:
	uint64_t m_BaseAddress;
	VortexDeviceAddress* m_DeviceID;
	IVortexConfiguration* m_InitConfig;
	VortexConfigurationGroup** m_ConfigurationGroups;
	vector<uint8_t> m_ConfigDataBytes;
};

#endif

