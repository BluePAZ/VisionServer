#include "VortexTypes.h"

VortexMemoryAllocation::VortexMemoryAllocation(VortexDeviceAddress* MemoryDeviceID, uint64_t MemoryOffset, uint64_t MemorySize)
{
	m_DeviceID = MemoryDeviceID;
	SetMemoryOffset(MemoryOffset);
	SetMemorySize(MemorySize);
}

VortexMemoryAllocation::~VortexMemoryAllocation(void)
{
}

VortexDeviceAddress* VortexMemoryAllocation::GetMemoryDeviceID()
{
	return m_DeviceID;
}

void VortexMemoryAllocation::SetMemoryDeviceID(VortexDeviceAddress* value)
{
	m_DeviceID = value;
}

void VortexMemoryAllocation::SetMemoryDeviceID(uint16_t value)
{
	m_DeviceID->SetDeviceID(value);
}

uint64_t VortexMemoryAllocation::GetMemoryOffset()
{
	return m_Address;
}

void VortexMemoryAllocation::SetMemoryOffset(uint64_t value)
{
	m_Address = value;
}

uint64_t VortexMemoryAllocation::GetMemorySize()
{
	return m_Size;
}

void VortexMemoryAllocation::SetMemorySize(uint64_t value)
{
	m_Size = value;
}