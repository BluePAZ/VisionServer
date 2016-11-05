#include "hog_engine_v1_00_a_histogram_pagetable_config.h"


hog_engine_v1_00_a_histogram_pagetable_config::hog_engine_v1_00_a_histogram_pagetable_config(void)
{
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_histogram_pagetable_config::~hog_engine_v1_00_a_histogram_pagetable_config(void)
{
	delete(m_Vcp);
}

vector<uint32_t> hog_engine_v1_00_a_histogram_pagetable_config::GetGroupIDs()
{
	return m_HistogramPageTableConfig.m_GroupID;
}

void hog_engine_v1_00_a_histogram_pagetable_config::SetGroupIDs(vector<uint32_t> values)
{
	// Clearing before populating else results in accumulating over time - ska - 11/21/2015
	m_HistogramPageTableConfig.m_GroupID.clear();

	for (int i = 0; i < values.size(); i++)
	{
		m_HistogramPageTableConfig.m_GroupID.push_back(values[i]);
	}
	
}

uint64_t hog_engine_v1_00_a_histogram_pagetable_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_histogram_pagetable_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_histogram_pagetable_config::GetConfiguration()
{
	m_Vcp->Clear();

	vector <uint8_t> ptdata = m_HistogramPageTableConfig.GetBytes();

	for (int i = 0; i < ptdata.size(); i++)
	{
		uint8_t ptcoeff = ptdata[i];
		m_Vcp->Add(ptcoeff);
	}

	return m_Vcp;
}

VortexEntityType hog_engine_v1_00_a_histogram_pagetable_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_histogram_pagetable_config::SetConfigurationType(VortexEntityType Type)
{
}

vector<uint32_t> hog_engine_v1_00_a_histogram_pagetable_config::CreateHOGHistogramPartition(uint16_t MaxImageWidth, uint16_t MaxImageHeight, uint16_t CellWidth, uint16_t CellHeight)
{
	uint16_t NumRows = MaxImageHeight / CellHeight;
	uint16_t NumCols = MaxImageWidth / CellWidth;

	uint32_t PTDepth = NumRows * NumCols;

	// upper 16 bit = hid
	// lower 16 bit = gid
	vector <uint32_t> PageTable;

	// Default mappings = Every cell has a gid and correponding hid initialized to 0
	for (int d = 0; d < PTDepth; d++)
	{
		//PageTable.push_back(((uint16_t)d & 0xFFFF) << 16);
		PageTable.push_back(((uint16_t)d & 0xFFFF));
	}

	return PageTable;

}
