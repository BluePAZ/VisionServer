#include "surf_ip_detector_v1_00_b_integral_image_config.h"


surf_ip_detector_v1_00_b_integral_image_config::surf_ip_detector_v1_00_b_integral_image_config(void)
{
	m_IntegralImageConfig.Opcode = 0;
	m_IntegralImageConfig.Width = 0;
	m_IntegralImageConfig.Height = 0;
	m_Vcp = new VortexConfigurationPacket();
}

surf_ip_detector_v1_00_b_integral_image_config::~surf_ip_detector_v1_00_b_integral_image_config(void)
{
	delete(m_Vcp);
}

uint8_t surf_ip_detector_v1_00_b_integral_image_config::GetOpcode()
{
	return m_IntegralImageConfig.Opcode;
}

void surf_ip_detector_v1_00_b_integral_image_config::SetOpcode(uint8_t value)
{
	m_IntegralImageConfig.Opcode = value;
}

uint16_t surf_ip_detector_v1_00_b_integral_image_config::GetWidth()
{
	return m_IntegralImageConfig.Width;
}

void surf_ip_detector_v1_00_b_integral_image_config::SetWidth(uint16_t value)
{
    m_IntegralImageConfig.Width = value;
}

uint16_t surf_ip_detector_v1_00_b_integral_image_config::GetHeight()
{
    return m_IntegralImageConfig.Height;
}

void surf_ip_detector_v1_00_b_integral_image_config::SetHeight(uint16_t value)
{
    m_IntegralImageConfig.Height = value;
}

uint64_t surf_ip_detector_v1_00_b_integral_image_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void surf_ip_detector_v1_00_b_integral_image_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* surf_ip_detector_v1_00_b_integral_image_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_IntegralImageConfig.GetBytes(),16);

	return m_Vcp;
}

VortexEntityType surf_ip_detector_v1_00_b_integral_image_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void surf_ip_detector_v1_00_b_integral_image_config::SetConfigurationType(VortexEntityType Type)
{
}
