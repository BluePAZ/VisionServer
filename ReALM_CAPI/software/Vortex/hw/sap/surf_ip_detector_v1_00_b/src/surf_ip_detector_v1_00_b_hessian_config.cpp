#include "surf_ip_detector_v1_00_b_hessian_config.h"


surf_ip_detector_v1_00_b_hessian_config::surf_ip_detector_v1_00_b_hessian_config(void)
{
	m_HessianConfig.Opcode = 0;
	m_HessianConfig.Width = 0;
	m_HessianConfig.Height = 0;
	m_Vcp = new VortexConfigurationPacket();
}

surf_ip_detector_v1_00_b_hessian_config::~surf_ip_detector_v1_00_b_hessian_config(void)
{
	delete(m_Vcp);
}

uint8_t surf_ip_detector_v1_00_b_hessian_config::GetOpcode()
{
	return m_HessianConfig.Opcode;
}

void surf_ip_detector_v1_00_b_hessian_config::SetOpcode(uint8_t value)
{
	m_HessianConfig.Opcode = value;
}

uint16_t surf_ip_detector_v1_00_b_hessian_config::GetWidth()
{
	return m_HessianConfig.Width;
}

void surf_ip_detector_v1_00_b_hessian_config::SetWidth(uint16_t value)
{
    m_HessianConfig.Width = value;
}

uint16_t surf_ip_detector_v1_00_b_hessian_config::GetHeight()
{
    return m_HessianConfig.Height;
}

void surf_ip_detector_v1_00_b_hessian_config::SetHeight(uint16_t value)
{
    m_HessianConfig.Height = value;
}


uint64_t surf_ip_detector_v1_00_b_hessian_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void surf_ip_detector_v1_00_b_hessian_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* surf_ip_detector_v1_00_b_hessian_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_HessianConfig.GetBytes(),16);

	return m_Vcp;
}

VortexEntityType surf_ip_detector_v1_00_b_hessian_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void surf_ip_detector_v1_00_b_hessian_config::SetConfigurationType(VortexEntityType Type)
{
}
