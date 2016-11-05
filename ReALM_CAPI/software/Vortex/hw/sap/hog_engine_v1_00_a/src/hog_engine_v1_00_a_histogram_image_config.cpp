#include "hog_engine_v1_00_a_histogram_image_config.h"


hog_engine_v1_00_a_histogram_image_config::hog_engine_v1_00_a_histogram_image_config(void)
{
	m_HistogramImageConfig.NumHistograms = 0;
	m_HistogramImageConfig.Width = 0;
	m_HistogramImageConfig.Height = 0;
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_histogram_image_config::~hog_engine_v1_00_a_histogram_image_config(void)
{
	delete(m_Vcp);
}


uint16_t hog_engine_v1_00_a_histogram_image_config::GetHistograms()
{
	return m_HistogramImageConfig.NumHistograms;
}

void hog_engine_v1_00_a_histogram_image_config::SetHistograms(uint16_t value)
{
	m_HistogramImageConfig.NumHistograms = value;
}


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
