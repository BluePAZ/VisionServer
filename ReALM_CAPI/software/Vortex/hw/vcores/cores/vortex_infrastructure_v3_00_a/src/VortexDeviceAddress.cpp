#include "VortexTypes.h"

VortexDeviceAddress::VortexDeviceAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
    m_DeviceID = ((uint16_t)BusID << 8) | ((uint16_t)SwitchID << 3) | ((uint16_t)PortID);
}

VortexDeviceAddress::VortexDeviceAddress(uint16_t DeviceID)
{
    m_DeviceID = DeviceID;
}

VortexDeviceAddress::VortexDeviceAddress(void)
{
    m_DeviceID = 0xFF;
}

VortexDeviceAddress::~VortexDeviceAddress(void)
{
}

uint8_t* VortexDeviceAddress::GetBytes()
{
	return (uint8_t*)&m_DeviceID;
}

void VortexDeviceAddress::SetBusID(uint8_t value)
{
	((uint8_t*)&m_DeviceID)[1] = value;
}

uint8_t VortexDeviceAddress::GetBusID()
{
	return ((uint8_t*)&m_DeviceID)[1];
}

void VortexDeviceAddress::SetSwitchID(uint8_t value)
{
	((uint8_t*)&m_DeviceID)[0] &= 0x07;
	((uint8_t*)&m_DeviceID)[0] |= (value << 3) & ~0x7;
}

uint8_t VortexDeviceAddress::GetSwitchID()
{
	return ((uint8_t*)&m_DeviceID)[0] >> 3;
}

void VortexDeviceAddress::SetPortID(uint8_t value)
{
	((uint8_t*)&m_DeviceID)[0] &= ~0x07;
	((uint8_t*)&m_DeviceID)[0] |= value & 0x7;
}

uint8_t VortexDeviceAddress::GetPortID()
{
	return ((uint8_t*)&m_DeviceID)[0] & 0x7;
}

void VortexDeviceAddress::SetDeviceID(uint16_t value)
{
	m_DeviceID = value;
}

uint16_t VortexDeviceAddress::GetDeviceID()
{
	return m_DeviceID;
}


