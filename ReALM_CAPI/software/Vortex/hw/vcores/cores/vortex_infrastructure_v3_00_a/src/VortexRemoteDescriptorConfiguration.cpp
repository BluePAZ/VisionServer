#include "VortexTypes.h"


VortexRemoteDescriptorConfiguration::VortexRemoteDescriptorConfiguration(uint8_t BusID, uint8_t SwitchID, uint8_t PortID, uint64_t Address, uint16_t FlowID, uint64_t Length, bool IsLast)
	:VortexDescriptorConfiguration(BusID,SwitchID,PortID,Address,FlowID,Length)
{
	m_FlowID = FlowID;
	//m_DeviceID = new VortexDeviceAddress(BusID,SwitchID,PortID);
	m_Address = Address;
	m_Length = Length;
	m_IsLast = IsLast;
	//m_Vcp = new VortexConfigurationPacket();
}

VortexRemoteDescriptorConfiguration::~VortexRemoteDescriptorConfiguration(void)
{
	//delete(m_DeviceID);
	//delete(m_Vcp);
}

void VortexRemoteDescriptorConfiguration::SetLastFlag(bool Last)
{
	m_IsLast = Last;
}

bool VortexRemoteDescriptorConfiguration::GetLastFlag()
{
	return m_IsLast;
}

VortexConfigurationPacket* VortexRemoteDescriptorConfiguration::GetConfiguration()
{
	uint8_t data[16];
	*((uint32_t*)&data[0])  = (uint32_t)(GetLength() & 0xFFFFFFFF);
	*((uint32_t*)&data[4])  = (uint32_t)((GetLength() >> 32) & 0xF) | (uint32_t)((GetAddress() & 0x000000000FFFFFFF) << 4);
	*((uint32_t*)&data[8])  = (uint32_t)((GetAddress() >> 28) & 0xFF) | ((uint32_t)(GetDeviceID()->GetDeviceID()) << 8) | ((uint32_t)GetFlowID() << 24);
	*((uint32_t*)&data[12]) = (uint32_t)((GetFlowID() >> 8) & 0x3) | (uint32_t)((GetAddress() >> 36) & 0x000000000FFFFFFF) | (uint32_t)((GetLastFlag()) ? 0x80000000 : 0x0);
	
	m_Vcp->Clear();
	m_Vcp->Add(data,16);

	return m_Vcp;
}