#include "VortexSOPTypes.h"


VortexOperatorLinkConfiguration::VortexOperatorLinkConfiguration(uint64_t BaseAddress, int IngressBitWidth, uint64_t TransferSizeBytes, VortexEntityType ConfigType)
{
	SetBaseAddress(BaseAddress);
	SetConfigurationType(ConfigType);
	
	m_IngressBitWidth = IngressBitWidth;
	m_TransferSizeBytes = TransferSizeBytes;

	vcp = new VortexConfigurationPacket();
	vcp->SetAddress(GetIndex());
}

VortexOperatorLinkConfiguration::~VortexOperatorLinkConfiguration(void)
{
	delete(vcp);
}

uint64_t VortexOperatorLinkConfiguration::GetIndex()
{
	return m_BaseAddress;
}

void VortexOperatorLinkConfiguration::SetIndex(uint64_t Index)
{
	m_BaseAddress = Index;
}

void VortexOperatorLinkConfiguration::SetBaseAddress(uint64_t value)
{
	m_BaseAddress = value;
}

int VortexOperatorLinkConfiguration::GetIngressBitWidth()
{
	return m_IngressBitWidth;
}

uint64_t VortexOperatorLinkConfiguration::GetTransferSizeBytes()
{
	return m_TransferSizeBytes;
}

void VortexOperatorLinkConfiguration::SetTransferSizeBytes(uint64_t value)
{
	m_TransferSizeBytes = value;
}

VortexEntityType VortexOperatorLinkConfiguration::GetConfigurationType()
{
	return m_ConfigurationType;
}

void VortexOperatorLinkConfiguration::SetConfigurationType(VortexEntityType ConfigType)
{
	m_ConfigurationType = ConfigType;
}

VortexConfigurationPacket* VortexOperatorLinkConfiguration::GetConfiguration()
{
    uint32_t numTransfers = (uint32_t)(GetTransferSizeBytes() / (uint64_t)(GetIngressBitWidth() / 8));
	
	vcp->Clear();
	vcp->Add(((uint8_t*)&numTransfers)[0]);
	vcp->Add(((uint8_t*)&numTransfers)[1]);
	vcp->Add(((uint8_t*)&numTransfers)[2]);
	vcp->Add(((uint8_t*)&numTransfers)[3]);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
	vcp->Add(0);
    
    return vcp;
}
