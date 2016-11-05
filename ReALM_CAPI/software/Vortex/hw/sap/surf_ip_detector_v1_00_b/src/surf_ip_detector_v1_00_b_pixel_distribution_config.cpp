#include "surf_ip_detector_v1_00_b_pixel_distribution_config.h"

surf_ip_detector_v1_00_b_pixel_distribution_config::surf_ip_detector_v1_00_b_pixel_distribution_config(uint64_t BaseAddress, uint16_t Width, uint16_t Height, uint8_t Opcode)
{
	m_Vcp = new VortexConfigurationPacket();
	m_PixelDistributionConfig.Opcode = Opcode;
	m_PixelDistributionConfig.Width = Width;
	m_PixelDistributionConfig.Height = Height;
	m_Vcp->SetAddress(BaseAddress);
}

surf_ip_detector_v1_00_b_pixel_distribution_config::surf_ip_detector_v1_00_b_pixel_distribution_config(void)
{
	m_Vcp = new VortexConfigurationPacket();
	m_PixelDistributionConfig.Opcode = 0;
	m_PixelDistributionConfig.Width = 0;
	m_PixelDistributionConfig.Height = 0;
}

surf_ip_detector_v1_00_b_pixel_distribution_config::~surf_ip_detector_v1_00_b_pixel_distribution_config(void)
{
	delete(m_Vcp);
}

uint8_t surf_ip_detector_v1_00_b_pixel_distribution_config::GetOpcode()
{
	return m_PixelDistributionConfig.Opcode;
}

void surf_ip_detector_v1_00_b_pixel_distribution_config::SetOpcode(uint8_t value)
{
	m_PixelDistributionConfig.Opcode = value;
}

uint16_t surf_ip_detector_v1_00_b_pixel_distribution_config::GetWidth()
{
	return m_PixelDistributionConfig.Width;
}

void surf_ip_detector_v1_00_b_pixel_distribution_config::SetWidth(uint16_t value)
{
    m_PixelDistributionConfig.Width = value;
}

uint16_t surf_ip_detector_v1_00_b_pixel_distribution_config::GetHeight()
{
    return m_PixelDistributionConfig.Height;
}

void surf_ip_detector_v1_00_b_pixel_distribution_config::SetHeight(uint16_t value)
{
    m_PixelDistributionConfig.Height = value;
}

uint64_t surf_ip_detector_v1_00_b_pixel_distribution_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void surf_ip_detector_v1_00_b_pixel_distribution_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* surf_ip_detector_v1_00_b_pixel_distribution_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_PixelDistributionConfig.GetBytes(),16);

	return m_Vcp;
}

VortexEntityType surf_ip_detector_v1_00_b_pixel_distribution_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void surf_ip_detector_v1_00_b_pixel_distribution_config::SetConfigurationType(VortexEntityType Type)
{
}