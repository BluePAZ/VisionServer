
#include <stdio.h>
#include <stdlib.h>

#include "nif_capi_v2_00_b.h"
#include "cxl_wrapper.h"

using namespace std;

bool nifsap_capi_v2_00_b::m_FlowIDList[1024];

nifsap_capi_v2_00_b::nifsap_capi_v2_00_b(bool SimulationMode, int BoardID, string ConfigFilename, uint8_t SWBusID, uint8_t SWSwitchID, uint8_t SWPortID)
{
	m_SWDeviceID = new VortexDeviceAddress(SWBusID,SWSwitchID,SWPortID);
	m_HWDeviceID = new VortexDeviceAddress(0,0,0);

	for (int i = 0; i < 1023; i++)
		m_FlowIDList[i] = (i<64) ? false : true;

}

nifsap_capi_v2_00_b::nifsap_capi_v2_00_b(bool SimulationMode, int BoardID, string ConfigFilename)
{
	for (int i = 0; i < 1023; i++)
		m_FlowIDList[i] = (i<64) ? false : true;
}

nifsap_capi_v2_00_b::~nifsap_capi_v2_00_b(void)
{
	Deinitialize();

	for(std::vector<VortexDeviceAddress*>::iterator it = m_MemoryInterfaces.begin(); it != m_MemoryInterfaces.end(); ++it)
		delete (*it);

	delete(m_SWDeviceID);
}

void nifsap_capi_v2_00_b::AddMemoryInterface(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_HWDeviceID->SetBusID(BusID);
	m_HWDeviceID->SetSwitchID(SwitchID);
	m_HWDeviceID->SetPortID(PortID);

	m_MemoryInterfaces.clear();
	m_MemoryInterfaces.push_back(m_HWDeviceID);
}

void nifsap_capi_v2_00_b::ResetSAP(VortexDeviceAddress* SAP)
{
	uint8_t ResetKeyValue[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF };

	VortexConfigurationGroup* vcg = new VortexConfigurationGroup(SAP, new VortexConfigurationPacket(ResetKeyValue,16,0xF0),VortexEntityType::NIFSAP);

	Configure(vcg);

	delete(vcg);
}

bool nifsap_capi_v2_00_b::Initialize(int BoardID)
{
	if (cxl_initialize() != 0)
	{
        printf("ERROR Intializing AFU\n");
		return false;
	}
    /*
	uint64_t testData = 0xDEAD0000BEEF0000;
	//Testing new MMIO functionality
	testData = mmio_read64(0x002000);
	testData = 0xDEAD0000BEEF0000;
    mmio_write64(0x002800,testData);

    testData = mmio_read64(0x000000);
    testData = mmio_read64(0x000800);
    mmio_write64(0x000800,testData);
    */
	return true;
}

void nifsap_capi_v2_00_b::Deinitialize()
{
	cxl_deinitialize();
}

void nifsap_capi_v2_00_b::Configure(VortexConfigurationGroup* ConfigGroup)
{
	if (ConfigGroup == NULL)
		return;

	int NumConfigurations;
	IVortexConfiguration** Configs = ConfigGroup->GetConfigurations(NumConfigurations);

	for (int i=0; i<NumConfigurations; i++)
	{
		uint16_t ConfigFlow;

		switch (Configs[i]->GetConfigurationType())
		{
		case VortexEntityType::NIFSAP	: ConfigFlow = 20; break;
		case VortexEntityType::NIFSOP	: ConfigFlow = 21; break;
		case VortexEntityType::SOP		: ConfigFlow = 22; break;
		case VortexEntityType::SAP		: ConfigFlow = 12; break;
		default							: ConfigFlow = 12; break;
		}

		int NumConfigBytes;
		uint8_t* ConfigBytes = Configs[i]->GetConfiguration()->GetDataBytes(NumConfigBytes);
		WriteStream(ConfigBytes,NumConfigBytes,ConfigFlow,ConfigGroup->GetDeviceID(),Configs[i]->GetConfiguration()->GetAddress());
	}
}

void nifsap_capi_v2_00_b::Configure(VortexConfigurationGroup* Configurations[], int NumConfigurations)
{
	for (int i=0; i<NumConfigurations; i++)
		Configure(Configurations[i]);
}

void nifsap_capi_v2_00_b::Configure(uint16_t FlowID, uint64_t Length, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors)
{
}

bool nifsap_capi_v2_00_b::WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID)
{
	return false;
}

bool nifsap_capi_v2_00_b::WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors)
{
	bool completionRequired = false;
    VortexMessage* commandMessage = VortexMessage::CreateDMACommandMessage(SourceBuffer,VortexTransactionType::WRITE_DMA,Length,FlowID,m_SWDeviceID,m_HWDeviceID,completionRequired,Descriptors);
	/*
    if (cbInfo != NULL && commandMessage != NULL)
        RegisterContinuation(commandMessage->MessageHeader->TransactionID, cbInfo);
	*/
    SendMessage(commandMessage);
    VortexMessage* responseMessage = WaitMessage(-1);

	delete(commandMessage);
	delete(responseMessage);

	return true;
}

bool nifsap_capi_v2_00_b::WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* TargetDevice, uint64_t TargetAddress)
{
	bool completionRequired = true;

	VortexRemoteDescriptorConfiguration** Descriptors = new VortexRemoteDescriptorConfiguration*[1];
	Descriptors[0] = new VortexRemoteDescriptorConfiguration(TargetDevice->GetBusID(),TargetDevice->GetSwitchID(),TargetDevice->GetPortID(),TargetAddress,FlowID,Length,true);

	VortexMessage* commandMessage = VortexMessage::CreateDMACommandMessage(SourceBuffer,VortexTransactionType::WRITE_DMA,Length,FlowID,m_SWDeviceID,m_HWDeviceID,completionRequired,Descriptors);
	/*
    if (cbInfo != NULL && commandMessage != NULL)
        RegisterContinuation(commandMessage->MessageHeader->TransactionID, cbInfo);
	*/
    SendMessage(commandMessage);
    VortexMessage* responseMessage = WaitMessage(-1);

	delete(commandMessage);
	delete(responseMessage);
	delete(Descriptors[0]);
	delete[](Descriptors);

	return true;
}

bool nifsap_capi_v2_00_b::ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID)
{
	return false;
}

bool nifsap_capi_v2_00_b::ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors)
{
	//bool CompletionRequired = ((cbInfo != NULL) && (cbInfo->Callback != NULL)) ? true : false;
	bool CompletionRequired = false;
	VortexMessage* commandMessage = VortexMessage::CreateDMACommandMessage(TargetBuffer,VortexTransactionType::READ_DMA,Length,FlowID,m_SWDeviceID,m_HWDeviceID,CompletionRequired,Descriptors);
	/*
    if (cbInfo != NULL && commandMessage != NULL)
        RegisterContinuation(commandMessage->MessageHeader->TransactionID, cbInfo);
	*/
    SendMessage(commandMessage);
    VortexMessage* responseMessage = WaitMessage(-1);

	delete(commandMessage);
	delete(responseMessage);

	return true;
}

bool nifsap_capi_v2_00_b::ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* SourceDevice, uint64_t SourceAddress)
{
	bool CompletionRequired = false;

	VortexRemoteDescriptorConfiguration** Descriptors = new VortexRemoteDescriptorConfiguration*[1];
	Descriptors[0] = new VortexRemoteDescriptorConfiguration(SourceDevice->GetBusID(),SourceDevice->GetSwitchID(),SourceDevice->GetPortID(),SourceAddress,FlowID,Length,true);

	VortexMessage* commandMessage = VortexMessage::CreateDMACommandMessage(TargetBuffer,VortexTransactionType::READ_DMA,Length,FlowID,m_SWDeviceID,m_HWDeviceID,CompletionRequired,Descriptors);

    SendMessage(commandMessage);
    VortexMessage* responseMessage = WaitMessage(-1);

	delete(commandMessage);
	delete(responseMessage);
	delete(Descriptors[0]);
	delete[](Descriptors);

	return true;
}

bool nifsap_capi_v2_00_b::DMA(VortexTransferDescriptor* Descriptor)
{
	return false;
}

bool nifsap_capi_v2_00_b::DMA(VortexTransferDescriptor* Descriptor, int TimeoutMilliseconds)
{
	return false;
}

void nifsap_capi_v2_00_b::SendMessage(VortexMessage* Message)
{
	HostMessageSendStatus status;
    int messageNumBytes;
	Message->SetSourceDeviceID(m_SWDeviceID);
    uint8_t* messageBytes = Message->GetBytes(messageNumBytes);

    while (!((status = GetHostMessageSendQueueStatus()).MessageSlotAvailable))
#ifndef _WIN32
        usleep(1000);
#else
		;
#endif
    HostMessageSendBytes(messageBytes,messageNumBytes);
    /*
    for (int i=0; i<messageNumBytes; i+=16)
    {
        mmio_write64(0x10000,*((uint64_t*)&messageBytes[i]));
        mmio_write64(0x10008,*((uint64_t*)&messageBytes[i+8]));
    }
    */
    //delete[](messageBytes);
}

VortexMessage* nifsap_capi_v2_00_b::WaitMessage(int TimeoutMilliseconds)
{
	HostMessageReceiveStatus status;
    int messageNumBytes;
    uint8_t* messageBytes;

	while (!((status = GetHostMessageReceiveQueueStatus()).MessageAvailable))
	{
		TimeoutMilliseconds -= 1;
        #ifndef _WIN32
			usleep(1000);
		#else
			;
		#endif

		if (TimeoutMilliseconds == 0)
			return NULL;
	}

    messageBytes = HostMessageRecvBytes(status.MessageSizeBytes);
    VortexMessage* message = new VortexMessage(messageBytes);

    delete[](messageBytes);

	return message;
}

VortexMemoryAllocation* nifsap_capi_v2_00_b::AllocateMemory(uint64_t Size)
{
	if (m_MemoryInterfaces.size() == 0)
		return NULL;

	int MemoryInterfaceIndex = 0;
	void* ptr = Allocate(Size,CAPI_CACHELINE_SIZE);

	VortexMemoryAllocation* Allocation = new VortexMemoryAllocation(m_MemoryInterfaces[MemoryInterfaceIndex],(uint64_t)ptr,Size);

	return Allocation;
}

void nifsap_capi_v2_00_b::DeallocateMemory(VortexMemoryAllocation* Hndl)
{
	Deallocate((void*)Hndl->GetMemoryOffset());
	delete(Hndl);
}

uint16_t nifsap_capi_v2_00_b::AllocateFlowID(int NumConsecutive)
{
	vector<int> candidates;

	for (int i=64; i<1023; i++)
	{
        if (m_FlowIDList[i])
        {
            m_FlowIDList[i] = false;
            candidates.push_back(i);
            if (candidates.size() == NumConsecutive)
                return candidates[0];
        }
        else
        {
            for(std::vector<int>::iterator it = candidates.begin(); it != candidates.end(); ++it)
                m_FlowIDList[*it] = true;
        }
    }

	return 0;
}

void nifsap_capi_v2_00_b::DeallocateFlowID(uint16_t FlowID)
{
    if (FlowID < 64)
        return;

	m_FlowIDList[FlowID] = true;
}


void* nifsap_capi_v2_00_b::Allocate(int Size, int Alignment)
{
	void *p;
	#ifndef _WIN32
		if (posix_memalign(&p, Alignment, Size) != 0)
			p = NULL;
	#else
		p = _aligned_malloc(Size, Alignment);
	#endif
        if (!p)
			throw std::bad_alloc();
		return p;
	}

void nifsap_capi_v2_00_b::Deallocate(void* p)
{
	#ifndef _WIN32
		free(p);
	#else
		_aligned_free(p);
	#endif
}



