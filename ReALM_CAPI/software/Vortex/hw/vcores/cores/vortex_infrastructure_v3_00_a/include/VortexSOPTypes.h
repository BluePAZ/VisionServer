#ifndef __VORTEX_SOP_TYPES_H__
#define __VORTEX_SOP_TYPES_H__

#include "VortexTypes.h"

class VortexOperatorLinkConfiguration;
class VortexOperatorLink;
class VortexSOPInputPort;
class VortexSOPOutputPort;
class VortexSOPControlUnit;
class VortexSOPControlUnitConfiguration;

class VortexOperatorLink
{
public:
	VortexOperatorLink(uint64_t BaseAddress, int IngressBitWidth);
	~VortexOperatorLink(void);

	uint64_t GetBaseAddress();
	int GetIngressBitWidth();

	VortexOperatorLinkConfiguration* GetIndexConfiguration(int index);
	void SetIndexConfiguration(int index, uint64_t TransferSizeBytes, VortexEntityType ConfigType = VortexEntityType::SOP);

	vector<IVortexConfiguration*> GetConfiguration();
private:
	uint64_t m_BaseAddress;
	int m_IngressBitWidth;
	vector<VortexOperatorLinkConfiguration*> m_Configurations;
};

class VortexOperatorLinkConfiguration : public IVortexConfiguration
{
public:
	VortexOperatorLinkConfiguration(uint64_t BaseAddress, int IngressBitWidth, uint64_t TransferSizeBytes, VortexEntityType ConfigType = VortexEntityType::SOP);
	~VortexOperatorLinkConfiguration(void);

	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();
	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);

	void SetBaseAddress(uint64_t value);

	int GetIngressBitWidth();

	uint64_t GetTransferSizeBytes();
	void SetTransferSizeBytes(uint64_t value);

	
private:
	uint64_t m_BaseAddress;
	int m_IngressBitWidth;
	uint64_t m_TransferSizeBytes;
	VortexEntityType m_ConfigurationType;
	VortexConfigurationPacket* vcp;
};

class VortexSOPInputPort
{
public:
	VortexSOPInputPort(void);
	~VortexSOPInputPort(void);
};

class VortexSOPOutputPort
{
public:
	VortexSOPOutputPort(void);
	~VortexSOPOutputPort(void);
};

class VortexSOPControlUnit
{
public:
	VortexSOPControlUnit(void);
	~VortexSOPControlUnit(void);
};

typedef struct _SOPControlUnitConfig {
    uint16_t VirtualOpcode  ;
    uint16_t PhysicalOpcode ;
    
    uint8_t Bytes[16];

    uint8_t* GetBytes()
    {
        *((uint64_t*)&Bytes[0]) =   (((uint64_t)VirtualOpcode	& 0x07) << 0 ) |
                                    (((uint64_t)PhysicalOpcode	& 0xFFFF) << 3);

        *((uint64_t*)&Bytes[8]) =   0;

        return Bytes;
    }
} SOPControlUnitConfig;

class VortexSOPControlUnitConfiguration : public IVortexConfiguration
{
public:
	VortexSOPControlUnitConfiguration(uint64_t BaseAddress, uint16_t VirtualOpcode, uint16_t PhysicalOpcode);
	VortexSOPControlUnitConfiguration(void);
	~VortexSOPControlUnitConfiguration(void);

	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();
	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);

	uint16_t GetPhysicalOpcode();
	void SetPhysicalOpcode(uint16_t value);

	uint16_t GetVirtualOpcode();
	void SetVirtualOpcode(uint16_t value);
private:

	VortexConfigurationPacket* m_Vcp;
	SOPControlUnitConfig m_SOPControllerConfig;
};

#endif