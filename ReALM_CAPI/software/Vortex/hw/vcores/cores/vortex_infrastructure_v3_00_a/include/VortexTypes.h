#ifndef __VORTEX_TYPES_H__
#define __VORTEX_TYPES_H__

#define CAPI_CACHELINE_SIZE 128

#include <stdint.h>
#include <string>
#include <vector>
#include "aligned_allocator.h"

using namespace std;
using namespace util;

class VortexConfigurationPacket;
class VortexDeviceAddress;
class IVortexConfiguration;
class VortexGenericConfiguration;
class VortexConfigurationGroup;
class VortexRemoteDescriptorConfiguration;
class VortexTransferDescriptor;
class VortexVariable;
class VortexMemoryAllocation;

enum class VortexEntityType {NIFSAP = 0x0, NIFSOP = 0x1, SOP = 0x2, SAP = 0x3, NONE = 0xF};

class IVortexConfiguration
{
public :
	virtual ~IVortexConfiguration(void) = 0;
	virtual VortexEntityType GetConfigurationType() = 0;
	virtual void SetConfigurationType(VortexEntityType Type) = 0;
	virtual uint64_t GetIndex() = 0;
	virtual void SetIndex(uint64_t Index) = 0;
	virtual VortexConfigurationPacket* GetConfiguration() = 0;
};

class VortexGenericConfiguration : public IVortexConfiguration
{
public:
	VortexGenericConfiguration(VortexConfigurationPacket* ConfigPacket, VortexEntityType ConfigType);
	~VortexGenericConfiguration(void);

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();
private:
	VortexEntityType m_ConfigurationType;
	uint64_t m_Index;
	VortexConfigurationPacket* m_Configuration;
};

class VortexConfigurationPacket
{
public:
	VortexConfigurationPacket();
	VortexConfigurationPacket(uint64_t Address);
	VortexConfigurationPacket(uint8_t Data[], int Length, uint64_t Address);
	~VortexConfigurationPacket(void);

	void Add(uint8_t Data[], int Length);
	void Add(uint8_t Data);
	void Clear();

	uint8_t* GetDataBytes(int& Length);
	string* GetConfigString(int& Length);
	uint64_t GetAddress();
	void SetAddress(uint64_t value);

private:
	std::vector<uint8_t,aligned_allocator<uint8_t,CAPI_CACHELINE_SIZE> > m_Data;
     uint64_t m_Address;
	 void MakeMultiple(int Size);
};

class VortexConfigurationGroup
{
public:
	VortexConfigurationGroup(VortexDeviceAddress* DeviceID);
	VortexConfigurationGroup(VortexDeviceAddress* DeviceID, VortexConfigurationPacket* VCP, VortexEntityType ConfigType);
	VortexConfigurationGroup(VortexDeviceAddress* DeviceID, IVortexConfiguration* Configuration);
	~VortexConfigurationGroup(void);

	IVortexConfiguration** GetConfigurations(int& NumConfigurations);
	IVortexConfiguration** GetConfigurations();
	void Insert(IVortexConfiguration* Configuration);
	void Insert(IVortexConfiguration* Configuration, uint64_t Index);
	void Insert(IVortexConfiguration* Configuration, uint64_t Index, VortexEntityType ConfigurationType);
	void Insert(VortexConfigurationPacket* Configuration, VortexEntityType ConfigurationType);
	void Insert(IVortexConfiguration* Configurations[], int NumConfigurations);
	void Insert(VortexConfigurationPacket* Configurations[], int NumConfigurations, VortexEntityType ConfigurationType);
	void Remove(IVortexConfiguration* Configuration);

	VortexDeviceAddress* GetDeviceID();
	void SetDeviceID(VortexDeviceAddress* value);

	int GetLength();
private:

	vector<IVortexConfiguration*> m_Configurations;
	VortexDeviceAddress* m_DeviceID;
	vector<VortexGenericConfiguration*> m_LocalGenericConfigurations;

};

class VortexDeviceAddress
{
public:
	VortexDeviceAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	VortexDeviceAddress(uint16_t DeviceID);
	VortexDeviceAddress(void);
	~VortexDeviceAddress(void);

	void SetBusID(uint8_t value);
	uint8_t GetBusID();

	void SetSwitchID(uint8_t value);
	uint8_t GetSwitchID();

	void SetPortID(uint8_t value);
	uint8_t GetPortID();

	void SetDeviceID(uint16_t value);
	uint16_t GetDeviceID();

	uint8_t* GetBytes();
private:
	uint16_t m_DeviceID;
};

class VortexDescriptorConfiguration : public IVortexConfiguration
{
public:
	VortexDescriptorConfiguration(uint8_t BusID, uint8_t SwitchID, uint8_t PortID, uint64_t Address, uint16_t FlowID, uint64_t Length, bool RNW, bool CompletionRequired);

	virtual ~VortexDescriptorConfiguration(void);

	VortexDeviceAddress* GetDeviceID();
	uint64_t GetAddress();
	uint64_t GetLength();
	uint16_t GetFlowID();

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();
protected:
	VortexDescriptorConfiguration(uint8_t BusID, uint8_t SwitchID, uint8_t PortID, uint64_t Address, uint16_t FlowID, uint64_t Length);
	uint16_t m_FlowID;
	VortexDeviceAddress* m_DeviceID;
	uint64_t m_Address;
	uint64_t m_Length;
	VortexConfigurationPacket* m_Vcp;
private:
	bool m_RNW;
	bool m_CompletionRequired;
};

class VortexRemoteDescriptorConfiguration : public VortexDescriptorConfiguration
{
public:
	VortexRemoteDescriptorConfiguration(uint8_t BusID, uint8_t SwitchID, uint8_t PortID, uint64_t Address, uint16_t FlowID, uint64_t Length, bool IsLast=false);
	~VortexRemoteDescriptorConfiguration(void);

	bool GetLastFlag();
	void SetLastFlag(bool Last);

	VortexConfigurationPacket* GetConfiguration();
private:
	bool m_IsLast;
};

class VortexTransferDescriptor
{
public:
	VortexTransferDescriptor(void);
	~VortexTransferDescriptor(void);
};

class VortexVariable
{
public:
	VortexVariable(string Name, void* Variable);
	~VortexVariable(void);

	string GetName();
	void SetName(string value);

	void* GetVariable();
	void SetVariable(void* value);
private:
	string m_Name;
	void* m_Variable;
};

class VortexMemoryAllocation
{
public:
	VortexMemoryAllocation(VortexDeviceAddress* MemoryDeviceID, uint64_t MemoryOffset, uint64_t MemorySize);
	~VortexMemoryAllocation(void);

	VortexDeviceAddress* GetMemoryDeviceID();
	void SetMemoryDeviceID(VortexDeviceAddress* value);
	void SetMemoryDeviceID(uint16_t value);

	uint64_t GetMemoryOffset();
	void SetMemoryOffset(uint64_t value);

	uint64_t GetMemorySize();
	void SetMemorySize(uint64_t value);
private:
	VortexDeviceAddress* m_DeviceID;
	uint64_t m_Address;
	uint64_t m_Size;
};


#endif
