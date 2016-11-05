#include "VortexSOPTypes.h"

VortexOperatorLink::VortexOperatorLink(uint64_t BaseAddress, int IngressBitWidth)
{
	m_BaseAddress = BaseAddress;
	m_IngressBitWidth = IngressBitWidth;
	//m_Configurations.resize(8);

	for(int i=0; i<8; i++)
		m_Configurations.push_back(NULL);
}

VortexOperatorLink::~VortexOperatorLink(void)
{
	for (int i = 0; i < m_Configurations.size(); i++)
	{
		delete(m_Configurations[i]);
	}
}

uint64_t VortexOperatorLink::GetBaseAddress()
{
	return m_BaseAddress;
}

int VortexOperatorLink::GetIngressBitWidth()
{
	return m_IngressBitWidth;
}

VortexOperatorLinkConfiguration* VortexOperatorLink::GetIndexConfiguration(int index)
{
	if (index > m_Configurations.size())
		return NULL;

	return m_Configurations[index];
}

void VortexOperatorLink::SetIndexConfiguration(int index, uint64_t TransferSizeBytes, VortexEntityType ConfigType)
{
	m_Configurations[index % 8] = new VortexOperatorLinkConfiguration(GetBaseAddress(), GetIngressBitWidth(), TransferSizeBytes);
	m_Configurations[index % 8]->SetConfigurationType(ConfigType);
}

vector<IVortexConfiguration*> VortexOperatorLink::GetConfiguration()
{
	vector<IVortexConfiguration*> Configs;

	for (int i = 0; i < m_Configurations.size(); i++)
	{
        if (m_Configurations[i] != NULL)
        {
			m_Configurations[i]->SetBaseAddress(m_Configurations[i]->GetIndex() | (GetBaseAddress() + (uint64_t)i * 16));
            Configs.push_back(m_Configurations[i]);
		}
	}
	
	return Configs;
}