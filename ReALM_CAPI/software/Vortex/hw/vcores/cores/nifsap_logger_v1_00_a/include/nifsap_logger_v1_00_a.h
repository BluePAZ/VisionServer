#ifndef __NIFSAP_LOGGER_V1_00_A_H__
#define __NIFSAP_LOGGER_V1_00_A_H__

#include "VortexSAPTypes.h"

class nifsap_logger_v1_00_a : public IVortexNIFSAP
{
public:
	nifsap_logger_v1_00_a(bool SimulationMode, int BoardID, string ConfigFilename);
	~nifsap_logger_v1_00_a(void);

	void AddMemoryInterface(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	bool Initialize(int BoardID);
	void Deinitialize();
	void Configure(VortexConfigurationGroup* Configuration);
    void Configure(VortexConfigurationGroup* Configurations[], int NumConfigurations);
    void Configure(uint16_t FlowID, uint64_t Length, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors);
	bool WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID);
    bool WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors);
    bool WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* TargetDevice, uint64_t TargetAddress);
	bool ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID);
    bool ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors);
    bool ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* SourceDevice, uint64_t SourceAddress);
	bool DMA(VortexTransferDescriptor* Descriptor);
    bool DMA(VortexTransferDescriptor* Descriptor, int TimeoutMilliseconds);
	void SendMessage(VortexMessage* Message);
	VortexMessage* WaitMessage(int TimeoutMilliseconds = -1);
	VortexMemoryAllocation* AllocateMemory(uint64_t Size);
	void DeallocateMemory(VortexMemoryAllocation* Hndl);
	uint16_t AllocateFlowID();
	void DeallocateFlowID(uint16_t FlowID);
	void ResetSAP(VortexDeviceAddress* SAP);
private:
	string m_ConfigFilename;
	vector<uint16_t> m_FlowIDList;
	vector<VortexDeviceAddress*> m_MemoryInterfaces;
	void* Allocate(int Size, int Alignment);
	void Deallocate(void* p);
};

#endif

