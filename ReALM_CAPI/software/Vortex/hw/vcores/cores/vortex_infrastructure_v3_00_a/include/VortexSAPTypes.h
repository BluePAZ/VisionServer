#ifndef __VORTEX_SAP_TYPES_H__
#define __VORTEX_SAP_TYPES_H__

#include "VortexTypes.h"
#include "VortexMessage.h"
#include <map>

using namespace std;

class VortexAcceleratorEnvironment
{
public:
	VortexAcceleratorEnvironment(void);
	~VortexAcceleratorEnvironment(void);

	int GetIdentifier();
	void Insert(string Name, void* Variable);
	VortexVariable* Retrieve(string Name);
private:
	static int m_ID;
	map<string,VortexVariable*> m_VariableTable;
	int m_Identifier;
};

class IVortexNIFSAP
{
public:
	virtual ~IVortexNIFSAP(void) = 0;
	virtual void AddMemoryInterface(uint8_t BusID, uint8_t SwitchID, uint8_t PortID) = 0;
	virtual bool Initialize(int BoardID) = 0;
	virtual void Deinitialize() = 0;
	virtual void Configure(VortexConfigurationGroup* Configuration) = 0;
    virtual void Configure(VortexConfigurationGroup* Configurations[], int NumConfigurations) = 0;
    virtual void Configure(uint16_t FlowID, uint64_t Length, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors) = 0;
	virtual bool WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID) = 0;
    virtual bool WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors) = 0;
    virtual bool WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* TargetDevice, uint64_t TargetAddress) = 0;
	virtual bool ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID) = 0;
    virtual bool ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors) = 0;
    virtual bool ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* SourceDevice, uint64_t SourceAddress) = 0;
	virtual bool DMA(VortexTransferDescriptor* Descriptor) = 0;
    virtual bool DMA(VortexTransferDescriptor* Descriptor, int TimeoutMilliseconds) = 0;
	virtual void SendMessage(VortexMessage* Message) = 0;
	virtual VortexMessage* WaitMessage(int TimeoutMilliseconds = -1) = 0;
	virtual VortexMemoryAllocation* AllocateMemory(uint64_t Size) = 0;
	virtual void DeallocateMemory(VortexMemoryAllocation* Hndl) = 0;
	virtual uint16_t AllocateFlowID(int NumConsecutive = 1) = 0;
	virtual void DeallocateFlowID(uint16_t FlowID) = 0;
	virtual void ResetSAP(VortexDeviceAddress* SAP) = 0;
};

#endif
