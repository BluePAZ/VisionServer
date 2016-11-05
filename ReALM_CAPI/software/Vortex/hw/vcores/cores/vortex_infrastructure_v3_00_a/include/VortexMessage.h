#ifndef __VORTEX_MESSAGE_H__
#define __VORTEX_MESSAGE_H__

#include "VortexTypes.h"
#include <vector>

enum class VortexTransactionType { WRITE = 0, READ = 1, WRITE_DMA = 2, READ_DMA = 3, WRITE_SINGLE = 4, READ_SINGLE = 5 };
enum class VortexTransactionOption { BLOCKING = 0, NONBLOCKING = 1 };
enum class VortexTransactionError
{
	NONE	 					= 0,
	HANDLER_BUSY 				= 1,
	UNEXPECTED_REPLY 			= 2,
	UNSUPPORTED_CMD 			= 3,
	UNSUPPORTED_CMD_SLAVE_IF 	= 4,
	EGRESS_EARLY_TERMINATION 	= 5,
	SLAVE_EARLY_TERMINATION 	= 6,
	NO_HANDLER_RESPONSE			= 7,
	NO_MSG_HANDLER_EXISTS		= 8,
	NO_ASSOCIATIVE_HANDLER		= 9
};

enum class VortexMessageType
{
	MEMORY_DMA_REQUEST			= 0x20,
	MEMORY_DMA_COMPLETE			= 0x21,
	EXECUTE_REQUEST				= 0x22,
	EXECUTE_COMPLETE			= 0x23,
	UNSPECIFIED
};

class VortexMessage
{
public:
	static VortexMessage* CreateDMACommandMessage(void* data, VortexTransactionType transactionType, int length, int flowID, VortexDeviceAddress* sourceDeviceID, VortexDeviceAddress* targetDeviceID, bool completionRequired, VortexRemoteDescriptorConfiguration* remoteDescriptors[]);
	static uint16_t AllocateTransactionID();
	
	VortexMessage(VortexMessageType MessageType, VortexDeviceAddress* SourceDeviceID, VortexDeviceAddress* TargetDeviceID, bool CompletionRequired);
	VortexMessage(uint8_t* Data);
	~VortexMessage(void);

	VortexMessageType GetType();
	void SetType(VortexMessageType Type);

	VortexDeviceAddress* GetTargetDeviceID();
	void SetTargetDeviceID(VortexDeviceAddress* value);

	VortexDeviceAddress* GetSourceDeviceID();
	void SetSourceDeviceID(VortexDeviceAddress* value);

	uint16_t GetLength();
	void SetLength(uint16_t value);

	uint16_t GetTransactionID();
	void SetTransactionID(uint16_t value);

	bool GetCompletionRequiredFlag();
	void SetCompletionRequiredFlag(bool value);

	uint8_t* GetPayloadBytes(int& Length);
	uint8_t* GetPayloadBytes();
	uint8_t* GetHeaderBytes();
	uint8_t* GetBytes(int& Length);

	void AddBytes(uint8_t* Data, int Length);

private:
	static uint16_t m_TransactionID;

	VortexDeviceAddress* m_TargetDeviceID;
	VortexDeviceAddress* m_SourceDeviceID;
	vector<uint8_t> m_MessageBytes;
};


#endif