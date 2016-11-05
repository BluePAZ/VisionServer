#include "hog_engine_v1_00_a_histogram_image_config.h"


hog_engine_v1_00_a_histogram_image_config::hog_engine_v1_00_a_histogram_image_config(void)
{
	//m_HistogramImageConfig.Opcode = 0;
	m_HistogramImageConfig.Width = 0;
	m_HistogramImageConfig.Height = 0;
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_histogram_image_config::~hog_engine_v1_00_a_histogram_image_config(void)
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

uint16_t hog_engine_v1_00_a_histogram_image_config::GetWidth()
{
	return m_HistogramImageConfig.Width;
}

void hog_engine_v1_00_a_histogram_image_config::SetWidth(uint16_t value)
{
	m_HistogramImageConfig.Width = value;
}

uint16_t hog_engine_v1_00_a_histogram_image_config::GetHeight()
{
	return m_HistogramImageConfig.Height;
}

void hog_engine_v1_00_a_histogram_image_config::SetHeight(uint16_t value)
{
	m_HistogramImageConfig.Height = value;
}


uint64_t hog_engine_v1_00_a_histogram_image_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_histogram_image_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_histogram_image_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_HistogramImageConfig.GetBytes(), 16);

	return m_Vcp;
}

VortexEntityType hog_engine_v1_00_a_histogram_image_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_histogram_image_config::SetConfigurationType(VortexEntityType Type)
{
}
