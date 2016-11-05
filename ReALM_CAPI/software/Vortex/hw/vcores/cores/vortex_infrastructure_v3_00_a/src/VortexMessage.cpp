#include "VortexMessage.h"
#include <string>

uint16_t VortexMessage::m_TransactionID = 0;

VortexMessage::VortexMessage(VortexMessageType MessageType, VortexDeviceAddress* SourceDeviceID, VortexDeviceAddress* TargetDeviceID, bool CompletionRequired)
{
	if (SourceDeviceID == NULL)
		m_SourceDeviceID = new VortexDeviceAddress();
	else
		m_SourceDeviceID = new VortexDeviceAddress(SourceDeviceID->GetBusID(),SourceDeviceID->GetSwitchID(),SourceDeviceID->GetPortID());

	if (TargetDeviceID == NULL)
		m_TargetDeviceID = new VortexDeviceAddress();
	else
		m_TargetDeviceID = new VortexDeviceAddress(TargetDeviceID->GetBusID(),TargetDeviceID->GetSwitchID(),TargetDeviceID->GetPortID());

	m_MessageBytes.resize(16);

	SetLength(16);
	SetType(MessageType);
	SetTargetDeviceID(m_TargetDeviceID);
	SetSourceDeviceID(m_SourceDeviceID);
	SetCompletionRequiredFlag(CompletionRequired);
}

VortexMessage::VortexMessage(uint8_t* Data)
{
	m_SourceDeviceID = new VortexDeviceAddress();
	m_TargetDeviceID = new VortexDeviceAddress();

	AddBytes(Data,16);

	int remainingBytes = GetLength() - 16;

	AddBytes(&Data[16],remainingBytes);
}

VortexMessage::~VortexMessage(void)
{
	delete(m_SourceDeviceID);
	delete(m_TargetDeviceID);
}

void VortexMessage::AddBytes(uint8_t* Data, int Length)
{
	int newLength = GetLength() + Length;

	for (int i=0; i<Length; i++)
		m_MessageBytes.push_back(Data[i]);

	SetLength(newLength);
}

VortexMessageType VortexMessage::GetType()
{
	return (VortexMessageType)m_MessageBytes[0];
}

void VortexMessage::SetType(VortexMessageType Type)
{
	m_MessageBytes[0] = (uint8_t)Type;
}

VortexDeviceAddress* VortexMessage::GetTargetDeviceID()
{
	m_TargetDeviceID->SetDeviceID(*((uint16_t*)&m_MessageBytes[1]));
	return m_TargetDeviceID;
}

void VortexMessage::SetTargetDeviceID(VortexDeviceAddress* value)
{
	uint8_t* deviceID = value->GetBytes();

	if (value != NULL)
	{
		m_MessageBytes[1] = deviceID[0];
		m_MessageBytes[2] = deviceID[1];
	}
	else
	{
		m_MessageBytes[1] = 0xFF;
		m_MessageBytes[2] = 0xFF;
	}
}

VortexDeviceAddress* VortexMessage::GetSourceDeviceID()
{
	m_SourceDeviceID->SetDeviceID(*((uint16_t*)&m_MessageBytes[3]));
	return m_SourceDeviceID;
}

void VortexMessage::SetSourceDeviceID(VortexDeviceAddress* value)
{
	uint8_t* deviceID = value->GetBytes();
	if (value != NULL)
	{
		m_MessageBytes[3] = deviceID[0];
		m_MessageBytes[4] = deviceID[1];
	}
	else
	{
		m_MessageBytes[3] = 0xFF;
		m_MessageBytes[4] = 0xFF;
	}
}

uint16_t VortexMessage::GetLength()
{
	if (m_MessageBytes.size() < 16)
		return 0;

	return (uint16_t)m_MessageBytes[5] | (uint16_t)((m_MessageBytes[6] & 0x3) << 8);
}

void VortexMessage::SetLength(uint16_t value)
{
	m_MessageBytes[5]  = ((uint8_t*)&value)[0];
	m_MessageBytes[6] &= ~0x03;
	m_MessageBytes[6] |= ((uint8_t*)&value)[1] & 0x3;
}

bool VortexMessage::GetCompletionRequiredFlag()
{
	return (m_MessageBytes[6] & 0x4);
}

void VortexMessage::SetCompletionRequiredFlag(bool CompletionRequired)
{
	m_MessageBytes[6] &= 0x03;
	m_MessageBytes[6] |= (CompletionRequired) ? 0x4 : 0x0;
}

uint16_t VortexMessage::GetTransactionID()
{
	return (uint16_t)m_MessageBytes[8] | (uint16_t)((m_MessageBytes[9] & 0x3) << 8);
}

void VortexMessage::SetTransactionID(uint16_t value)
{
	m_MessageBytes[8] = ((uint8_t*)&value)[0];
	m_MessageBytes[9] = ((uint8_t*)&value)[1] & 0x3;
}

uint8_t* VortexMessage::GetHeaderBytes()
{
	return m_MessageBytes.data();
}

uint8_t* VortexMessage::GetPayloadBytes(int& Length)
{
	Length = GetLength() - 16;
	return &(m_MessageBytes.data()[16]);
}

uint8_t* VortexMessage::GetPayloadBytes()
{
	return &(m_MessageBytes.data()[16]);
}

uint8_t* VortexMessage::GetBytes(int& Length)
{
	Length = GetLength();
	return m_MessageBytes.data();
}

uint16_t VortexMessage::AllocateTransactionID()
{
	uint16_t tempID = m_TransactionID;
	m_TransactionID = (m_TransactionID == 0x3FF) ? 0 : m_TransactionID + 1;
	return tempID;
}

VortexMessage* VortexMessage::CreateDMACommandMessage(void* data, VortexTransactionType transactionType, int length, int flowID, VortexDeviceAddress* sourceDeviceID, VortexDeviceAddress* targetDeviceID, bool completionRequired, VortexRemoteDescriptorConfiguration* remoteDescriptors[])
{
	int dummy_int;
	bool RNW = transactionType == VortexTransactionType::READ_DMA;
    bool CompletionRequired = true;

	VortexMessage* mkMessage = new VortexMessage(
		VortexMessageType::MEMORY_DMA_REQUEST,
		sourceDeviceID,
		targetDeviceID,
		true);

	mkMessage->SetTransactionID(AllocateTransactionID());

	VortexDescriptorConfiguration* localDescriptor = new VortexDescriptorConfiguration(
		sourceDeviceID->GetBusID(),
		sourceDeviceID->GetSwitchID(),
		sourceDeviceID->GetPortID(),
		(uint64_t)data,
		flowID,
		length,
		RNW,
		CompletionRequired);

	mkMessage->AddBytes(localDescriptor->GetConfiguration()->GetDataBytes(dummy_int),16);

	if (remoteDescriptors == NULL)
		return mkMessage;

	for(int i=0; (remoteDescriptors[i] != NULL); i++)
	{
		mkMessage->AddBytes(remoteDescriptors[i]->GetConfiguration()->GetDataBytes(dummy_int),16);
		if (remoteDescriptors[i]->GetLastFlag())
            break;
	}

	delete(localDescriptor);

	return mkMessage;
}
