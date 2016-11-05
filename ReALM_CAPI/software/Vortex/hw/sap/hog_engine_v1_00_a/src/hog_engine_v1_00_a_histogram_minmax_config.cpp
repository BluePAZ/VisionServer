#include "hog_engine_v1_00_a_histogram_minmax_config.h"


hog_engine_v1_00_a_histogram_minmax_config::hog_engine_v1_00_a_histogram_minmax_config(void)
{
	//m_HistogramImageConfig.Opcode = 0;
	m_HistogramMinMaxConfig.Min = 0;
	m_HistogramMinMaxConfig.Max = 0;
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_histogram_minmax_config::~hog_engine_v1_00_a_histogram_minmax_config(void)
{
	delete(m_Vcp);
}

/*
uint8_t hog_engine_v1_00_a_histogram_image_config::GetOpcode()
{
	return m_HistogramImageConfig.Opcode;
}

void hog_engine_v1_00_a_histogram_image_config::SetOpcode(uint8_t value)
{
	m_HistogramImageConfig.Opcode = value;
}
*/

uint16_t hog_engine_v1_00_a_histogram_minmax_config::GetMin()
{
	return m_HistogramMinMaxConfig.Min;
}

void hog_engine_v1_00_a_histogram_minmax_config::SetMin(uint32_t value)
{
	m_HistogramMinMaxConfig.Min = value;
}

uint16_t hog_engine_v1_00_a_histogram_minmax_config::GetMax()
{
	return m_HistogramMinMaxConfig.Max;
}

void hog_engine_v1_00_a_histogram_minmax_config::SetMax(uint32_t value)
{
	m_HistogramMinMaxConfig.Max = value;
}


uint64_t hog_engine_v1_00_a_histogram_minmax_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_histogram_minmax_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_histogram_minmax_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_HistogramMinMaxConfig.GetBytes(), 16);

	return m_Vcp;
}

VortexEntityType hog_engine_v1_00_a_histogram_minmax_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_histogram_minmax_config::SetConfigurationType(VortexEntityType Type)
{
}
