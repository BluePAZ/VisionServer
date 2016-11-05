#include "VortexTypes.h"


VortexDescriptorConfiguration::VortexDescriptorConfiguration(uint8_t BusID, uint8_t SwitchID, uint8_t PortID, uint64_t Address, uint16_t FlowID, uint64_t Length, bool RNW, bool CompletionRequired)
{
	m_FlowID = FlowID;
	m_DeviceID = new VortexDeviceAddress(BusID,SwitchID,PortID);
	m_Address = Address;
	m_Length = Length;
	m_RNW = RNW;
	m_CompletionRequired = CompletionRequired;
	m_Vcp = new VortexConfigurationPacket();
}

VortexDescriptorConfiguration::VortexDescriptorConfiguration(uint8_t BusID, uint8_t SwitchID, uint8_t PortID, uint64_t Address, uint16_t FlowID, uint64_t Length)
{
	m_FlowID = FlowID;
	m_DeviceID = new VortexDeviceAddress(BusID,SwitchID,PortID);
	m_Address = Address;
	m_Length = Length;
	m_Vcp = new VortexConfigurationPacket();
}

VortexDescriptorConfiguration::~VortexDescriptorConfiguration(void)
{
	delete(m_DeviceID);
	delete(m_Vcp);
}

VortexEntityType VortexDescriptorConfiguration::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void VortexDescriptorConfiguration::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t VortexDescriptorConfiguration::GetIndex()
{
	return m_FlowID;
}

void VortexDescriptorConfiguration::SetIndex(uint64_t Index)
{
}

uint64_t VortexDescriptorConfiguration::GetAddress()
{
	return m_Address;
}

VortexDeviceAddress* VortexDescriptorConfiguration::GetDeviceID()
{
	return m_DeviceID;
}

uint64_t VortexDescriptorConfiguration::GetLength()
{
	return m_Length;
}

uint16_t VortexDescriptorConfiguration::GetFlowID()
{
	return m_FlowID;
}

VortexConfigurationPacket* VortexDescriptorConfiguration::GetConfiguration()
{
	uint8_t data[16];
	*((uint32_t*)&data[0])  = (uint32_t)(GetLength() & 0xFFFFFFFF);
	*((uint32_t*)&data[4])  = (uint32_t)((GetLength() >> 32) & 0xF) | (uint32_t)((GetDeviceID()->GetDeviceID() & 0xFFFF) << 4) | ((uint32_t)GetFlowID() << 20) | (uint32_t)((m_RNW) ? (0x1 << 30): 0x0) | (uint32_t)((m_CompletionRequired) ? (0x1 << 31) : 0x0);
	*((uint64_t*)&data[8])  = GetAddress();

	m_Vcp->Clear();
	m_Vcp->Add(data,16);

	return m_Vcp;
}
