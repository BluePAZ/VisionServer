#ifndef NIF_CAPI_V2_00_B_H_
#define NIF_CAPI_V2_00_B_H_

#include <unistd.h>
#include <stdint.h>
#include "VortexSAPTypes.h"

class nifsap_capi_v2_00_b : public IVortexNIFSAP
{
public:
	nifsap_capi_v2_00_b(bool SimulationMode, int BoardID, string ConfigFilename);
	nifsap_capi_v2_00_b(bool SimulationMode, int BoardID, string ConfigFilename, uint8_t SWBusID, uint8_t SWSwitchID, uint8_t SWPortID);
	~nifsap_capi_v2_00_b(void);

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
	uint16_t AllocateFlowID(int NumConsecutive = 1);
	void DeallocateFlowID(uint16_t FlowID);
	void ResetSAP(VortexDeviceAddress* SAP);
private:
	string m_ConfigFilename;
	static bool m_FlowIDList[1024];
	vector<VortexDeviceAddress*> m_MemoryInterfaces;
	void* Allocate(int Size, int Alignment);
	void Deallocate(void* p);
	VortexDeviceAddress* m_SWDeviceID;
	VortexDeviceAddress* m_HWDeviceID;
};

#endif /* NIF_CAPI_V2_00_B_H_ */

