#include "VortexTypes.h"


VortexConfigurationGroup::VortexConfigurationGroup(VortexDeviceAddress* DeviceID)
{
	m_DeviceID = DeviceID;
}

VortexConfigurationGroup::VortexConfigurationGroup(VortexDeviceAddress* DeviceID, VortexConfigurationPacket* VCP, VortexEntityType ConfigType)
{
	m_DeviceID = DeviceID;
	Insert(VCP,ConfigType);
}

VortexConfigurationGroup::VortexConfigurationGroup(VortexDeviceAddress* DeviceID, IVortexConfiguration* Configuration)
{
	m_DeviceID = DeviceID;
	m_Configurations.push_back(Configuration);
}

VortexConfigurationGroup::~VortexConfigurationGroup(void)
{
	for(std::vector<VortexGenericConfiguration*>::iterator it = m_LocalGenericConfigurations.begin(); it != m_LocalGenericConfigurations.end(); ++it)
		delete(*it);
}

IVortexConfiguration** VortexConfigurationGroup::GetConfigurations(int& NumConfigurations)
{
	NumConfigurations = m_Configurations.size();
	return m_Configurations.data();
}

IVortexConfiguration** VortexConfigurationGroup::GetConfigurations()
{
	return m_Configurations.data();
}

void VortexConfigurationGroup::Insert(IVortexConfiguration* Configuration)
{
	m_Configurations.push_back(Configuration);
}

void VortexConfigurationGroup::Insert(IVortexConfiguration* Configuration, uint64_t Index)
{
	Configuration->SetIndex(Index);
	Insert(Configuration);
}

void VortexConfigurationGroup::Insert(IVortexConfiguration* Configuration, uint64_t Index, VortexEntityType ConfigurationType)
{
	Configuration->SetIndex(Index);
	Configuration->SetConfigurationType(ConfigurationType);
	Insert(Configuration);
}

void VortexConfigurationGroup::Insert(VortexConfigurationPacket* Configuration, VortexEntityType ConfigurationType)
{
	VortexGenericConfiguration* vgc = new VortexGenericConfiguration(Configuration,ConfigurationType);
	m_LocalGenericConfigurations.push_back(vgc);
	m_Configurations.push_back(vgc);
}

void VortexConfigurationGroup::Insert(IVortexConfiguration* Configurations[], int NumConfigurations)
{
	m_Configurations.insert(m_Configurations.end(), Configurations, Configurations + NumConfigurations);
}

void VortexConfigurationGroup::Insert(VortexConfigurationPacket* Configurations[], int NumConfigurations, VortexEntityType ConfigurationType)
{
	for (int i=0; i<NumConfigurations; i++)
	{
		VortexGenericConfiguration* vgc = new VortexGenericConfiguration(Configurations[i],ConfigurationType);
		m_LocalGenericConfigurations.push_back(vgc);
		Insert(vgc);
	}
}

void VortexConfigurationGroup::Remove(IVortexConfiguration* Configuration)
{
	for (int i=0; i<m_Configurations.size(); i++)
		if (m_Configurations[i] == Configuration)
		{
			m_Configurations.erase(m_Configurations.begin() + i);
			return;
		}
}

VortexDeviceAddress* VortexConfigurationGroup::GetDeviceID()
{
	return m_DeviceID;
}

void VortexConfigurationGroup::SetDeviceID(VortexDeviceAddress* value)
{
	m_DeviceID = value;
}

int VortexConfigurationGroup::GetLength()
{
	return m_Configurations.size();
}
