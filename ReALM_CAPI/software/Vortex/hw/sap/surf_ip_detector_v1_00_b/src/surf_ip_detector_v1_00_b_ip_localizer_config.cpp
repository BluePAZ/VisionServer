#include "surf_ip_detector_v1_00_b_ip_localizer_config.h"


surf_ip_detector_v1_00_b_ip_localizer_config::surf_ip_detector_v1_00_b_ip_localizer_config(void)
{
	m_IPLocalizerConfig.Opcode = 0;
	m_IPLocalizerConfig.Width = 0;
	m_IPLocalizerConfig.Height = 0;
	m_IPLocalizerConfig.Threshold = 0;
	m_Vcp = new VortexConfigurationPacket();
}

surf_ip_detector_v1_00_b_ip_localizer_config::~surf_ip_detector_v1_00_b_ip_localizer_config(void)
{
	delete(m_Vcp);
}

uint8_t surf_ip_detector_v1_00_b_ip_localizer_config::GetOpcode()
{
	return m_IPLocalizerConfig.Opcode;
}

void surf_ip_detector_v1_00_b_ip_localizer_config::SetOpcode(uint8_t value)
{
	m_IPLocalizerConfig.Opcode = value;
}

uint16_t surf_ip_detector_v1_00_b_ip_localizer_config::GetWidth()
{
	return m_IPLocalizerConfig.Width;
}

void surf_ip_detector_v1_00_b_ip_localizer_config::SetWidth(uint16_t value)
{
    m_IPLocalizerConfig.Width = value;
}

uint16_t surf_ip_detector_v1_00_b_ip_localizer_config::GetHeight()
{
    return m_IPLocalizerConfig.Height;
}

void surf_ip_detector_v1_00_b_ip_localizer_config::SetHeight(uint16_t value)
{
    m_IPLocalizerConfig.Height = value;
}

uint32_t surf_ip_detector_v1_00_b_ip_localizer_config::GetThreshold()
{
	return m_IPLocalizerConfig.Threshold;
}

void surf_ip_detector_v1_00_b_ip_localizer_config::SetThreshold(uint32_t value)
{
	m_IPLocalizerConfig.Threshold = value;
}

uint64_t surf_ip_detector_v1_00_b_ip_localizer_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void surf_ip_detector_v1_00_b_ip_localizer_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* surf_ip_detector_v1_00_b_ip_localizer_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_IPLocalizerConfig.GetBytes(),16);

	return m_Vcp;
}

VortexEntityType surf_ip_detector_v1_00_b_ip_localizer_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void surf_ip_detector_v1_00_b_ip_localizer_config::SetConfigurationType(VortexEntityType Type)
{
}