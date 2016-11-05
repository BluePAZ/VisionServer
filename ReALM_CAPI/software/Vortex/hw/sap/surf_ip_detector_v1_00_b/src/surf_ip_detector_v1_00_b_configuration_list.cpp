#include "surf_ip_detector_v1_00_b_configuration_list.h"


surf_ip_detector_v1_00_b_configuration_list::surf_ip_detector_v1_00_b_configuration_list(VortexDeviceAddress* DeviceID, uint64_t BaseAddress)
{
	m_DeviceID = DeviceID;
	m_BaseAddress = BaseAddress;
	m_ConfigurationGroups = new VortexConfigurationGroup*[2];
	m_ConfigurationGroups[0] = new VortexConfigurationGroup(m_DeviceID);
	m_ConfigurationGroups[1] = new VortexConfigurationGroup(m_DeviceID);
}

surf_ip_detector_v1_00_b_configuration_list::~surf_ip_detector_v1_00_b_configuration_list(void)
{
}

uint64_t surf_ip_detector_v1_00_b_configuration_list::GetBaseAddress()
{
	return m_BaseAddress;
}

void surf_ip_detector_v1_00_b_configuration_list::SetBaseAddress(uint64_t value)
{
	m_BaseAddress = value;
}

VortexDeviceAddress* surf_ip_detector_v1_00_b_configuration_list::GetDeviceID()
{
	return m_DeviceID;
}

void surf_ip_detector_v1_00_b_configuration_list::SetDeviceID(VortexDeviceAddress* value)
{
	m_DeviceID = value;
}

vector<VortexConfigurationGroup*> surf_ip_detector_v1_00_b_configuration_list::GetConfigurationGroups()
{
	vector<VortexConfigurationGroup*> configGroups;
	configGroups.push_back(m_ConfigurationGroups[0]);
	configGroups.push_back(m_ConfigurationGroups[1]);
	return configGroups;
}

void surf_ip_detector_v1_00_b_configuration_list::Init()
{
	uint8_t ConfigBytes[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	delete m_ConfigurationGroups[0];
	m_ConfigurationGroups[0] = new VortexConfigurationGroup(m_DeviceID);

	delete m_ConfigurationGroups[1];
	m_ConfigurationGroups[1] = new VortexConfigurationGroup(m_DeviceID);

	m_ConfigurationGroups[0]->Insert(new VortexGenericConfiguration(new VortexConfigurationPacket(ConfigBytes,16,GetBaseAddress() + 0x10),VortexEntityType::SAP));
}

void surf_ip_detector_v1_00_b_configuration_list::Insert(IVortexConfiguration* Entry)
{
	int ConfigBytesLength;
	uint8_t* ConfigBytes = Entry->GetConfiguration()->GetDataBytes(ConfigBytesLength);
	VortexConfigurationPacket* vcp = new VortexConfigurationPacket(ConfigBytes,16,GetBaseAddress());
	VortexGenericConfiguration* vgc = new VortexGenericConfiguration(vcp,VortexEntityType::SAP);

	m_ConfigurationGroups[1]->Insert(vgc);
}

void surf_ip_detector_v1_00_b_configuration_list::Insert(vector<IVortexConfiguration*> Entries)
{
	for (int i=0; i<Entries.size(); i++)
		Insert(Entries[i]);
}
