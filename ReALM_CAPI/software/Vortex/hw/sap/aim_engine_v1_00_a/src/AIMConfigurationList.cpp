#include "AIMConfigurationList.h"


AIMConfigurationList::AIMConfigurationList(VortexDeviceAddress* DeviceID, uint64_t BaseAddress)
{
	m_DeviceID = DeviceID;
	m_BaseAddress = BaseAddress;
	m_ConfigurationGroups = new VortexConfigurationGroup*[2];
	m_ConfigurationGroups[0] = new VortexConfigurationGroup(m_DeviceID);
	m_ConfigurationGroups[1] = new VortexConfigurationGroup(m_DeviceID);
}

AIMConfigurationList::~AIMConfigurationList(void)
{
}

uint64_t AIMConfigurationList::GetBaseAddress()
{
	return m_BaseAddress;
}

void AIMConfigurationList::SetBaseAddress(uint64_t value)
{
	m_BaseAddress = value;
}

VortexDeviceAddress* AIMConfigurationList::GetDeviceID()
{
	return m_DeviceID;
}

void AIMConfigurationList::SetDeviceID(VortexDeviceAddress* value)
{
	m_DeviceID = value;
}

vector<VortexConfigurationGroup*> AIMConfigurationList::GetConfigurationGroups()
{
	vector<VortexConfigurationGroup*> configGroups;
	configGroups.push_back(m_ConfigurationGroups[0]);
	configGroups.push_back(m_ConfigurationGroups[1]);
	return configGroups;
}

void AIMConfigurationList::Init()
{
	uint8_t ConfigBytes[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	delete m_ConfigurationGroups[0];
	m_ConfigurationGroups[0] = new VortexConfigurationGroup(m_DeviceID);

	delete m_ConfigurationGroups[1];
	m_ConfigurationGroups[1] = new VortexConfigurationGroup(m_DeviceID);

	m_ConfigurationGroups[0]->Insert(new VortexGenericConfiguration(new VortexConfigurationPacket(ConfigBytes,16,GetBaseAddress() + 0x10),VortexEntityType::SAP));
}

void AIMConfigurationList::Insert(IVortexConfiguration* Entry)
{
	int ConfigBytesLength;
	uint8_t* ConfigBytes = Entry->GetConfiguration()->GetDataBytes(ConfigBytesLength);
	VortexConfigurationPacket* vcp = new VortexConfigurationPacket(ConfigBytes,16,GetBaseAddress());
	VortexGenericConfiguration* vgc = new VortexGenericConfiguration(vcp,VortexEntityType::SAP);
	
	m_ConfigurationGroups[1]->Insert(vgc);
}

void AIMConfigurationList::Insert(vector<IVortexConfiguration*> Entries)
{
	for (int i=0; i<Entries.size(); i++)
		Insert(Entries[i]);
}
