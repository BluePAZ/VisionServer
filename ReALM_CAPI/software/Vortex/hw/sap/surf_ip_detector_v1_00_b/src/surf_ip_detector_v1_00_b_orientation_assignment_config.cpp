#include "surf_ip_detector_v1_00_b_orientation_assignment_config.h"

surf_ip_detector_v1_00_b_orientation_assignment_config::surf_ip_detector_v1_00_b_orientation_assignment_config(uint16_t Width, uint16_t Height, uint8_t Opcode)
{
	m_OrientationAssignmentConfig.Opcode = Opcode;
	m_OrientationAssignmentConfig.Width = Width;
	m_OrientationAssignmentConfig.Height = Height;
	m_Vcp = new VortexConfigurationPacket();
}

surf_ip_detector_v1_00_b_orientation_assignment_config::surf_ip_detector_v1_00_b_orientation_assignment_config(void)
{
	m_OrientationAssignmentConfig.Opcode = 0;
	m_OrientationAssignmentConfig.Width = 0;
	m_OrientationAssignmentConfig.Height = 0;
	m_Vcp = new VortexConfigurationPacket();
}

surf_ip_detector_v1_00_b_orientation_assignment_config::~surf_ip_detector_v1_00_b_orientation_assignment_config(void)
{
	delete(m_Vcp);
}

uint8_t surf_ip_detector_v1_00_b_orientation_assignment_config::GetOpcode()
{
	return m_OrientationAssignmentConfig.Opcode;
}

void surf_ip_detector_v1_00_b_orientation_assignment_config::SetOpcode(uint8_t value)
{
	m_OrientationAssignmentConfig.Opcode = value;
}

uint16_t surf_ip_detector_v1_00_b_orientation_assignment_config::GetWidth()
{
	return m_OrientationAssignmentConfig.Width;
}

void surf_ip_detector_v1_00_b_orientation_assignment_config::SetWidth(uint16_t value)
{
    m_OrientationAssignmentConfig.Width = value;
}

uint16_t surf_ip_detector_v1_00_b_orientation_assignment_config::GetHeight()
{
    return m_OrientationAssignmentConfig.Height;
}

void surf_ip_detector_v1_00_b_orientation_assignment_config::SetHeight(uint16_t value)
{
    m_OrientationAssignmentConfig.Height = value;
}

uint64_t surf_ip_detector_v1_00_b_orientation_assignment_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void surf_ip_detector_v1_00_b_orientation_assignment_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* surf_ip_detector_v1_00_b_orientation_assignment_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_OrientationAssignmentConfig.GetBytes(),16);

	return m_Vcp;
}

VortexEntityType surf_ip_detector_v1_00_b_orientation_assignment_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void surf_ip_detector_v1_00_b_orientation_assignment_config::SetConfigurationType(VortexEntityType Type)
{
}