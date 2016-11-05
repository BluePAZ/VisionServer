#include "hog_engine_v1_00_a_controller_config.h"


hog_engine_v1_00_a_controller_config::hog_engine_v1_00_a_controller_config(void)
{
    m_ControllerConfig.FlowID = 0;
	m_ResultDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_controller_config::~hog_engine_v1_00_a_controller_config(void)
{
	delete(m_ResultDeviceID);
	delete(m_Vcp);
}

uint16_t hog_engine_v1_00_a_controller_config::GetWidth()
{
	return m_ControllerConfig.Width;
}

void hog_engine_v1_00_a_controller_config::SetWidth(uint16_t value)
{
    m_ControllerConfig.Width = value;
}

uint16_t hog_engine_v1_00_a_controller_config::GetHeight()
{
    return m_ControllerConfig.Height;
}

void hog_engine_v1_00_a_controller_config::SetHeight(uint16_t value)
{
    m_ControllerConfig.Height = value;
}

IMAGE_DESCRIPTOR_TYPE hog_engine_v1_00_a_controller_config::GetType()
{
	return (IMAGE_DESCRIPTOR_TYPE)m_ControllerConfig.Type;
}

void hog_engine_v1_00_a_controller_config::SetType(IMAGE_DESCRIPTOR_TYPE value)
{
    m_ControllerConfig.Type = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_COLOR hog_engine_v1_00_a_controller_config::GetFormatColor()
{
	return (IMAGE_DESCRIPTOR_FORMAT_COLOR)m_ControllerConfig.Color;
}

void hog_engine_v1_00_a_controller_config::SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value)
{
    m_ControllerConfig.Color = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_DEPTH hog_engine_v1_00_a_controller_config::GetFormatDepth()
{
	return (IMAGE_DESCRIPTOR_FORMAT_DEPTH)m_ControllerConfig.Depth;
}

void hog_engine_v1_00_a_controller_config::SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value)
{
    m_ControllerConfig.Depth = (uint8_t)value;
}

void hog_engine_v1_00_a_controller_config::SetBaseFlowID(uint16_t value)
{
    m_ControllerConfig.FlowID = value;
}

uint16_t hog_engine_v1_00_a_controller_config::GetBaseFlowID()
{
    return m_ControllerConfig.FlowID;
}

uint16_t hog_engine_v1_00_a_controller_config::GetPixelFetchFlowID()
{
	return m_ControllerConfig.FlowID;
}

uint16_t hog_engine_v1_00_a_controller_config::GetResultStoreFlowID()
{
	return (m_ControllerConfig.FlowID == 0) ? 0 : m_ControllerConfig.FlowID + 2;
}

VortexDeviceAddress* hog_engine_v1_00_a_controller_config::GetResultDeviceID()
{
	return m_ResultDeviceID;
}

void hog_engine_v1_00_a_controller_config::SetResultDeviceID(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	delete(m_ResultDeviceID);
	m_ResultDeviceID = new VortexDeviceAddress(BusID,SwitchID,PortID);
	m_ControllerConfig.ResultDeviceID = m_ResultDeviceID->GetDeviceID();
}

uint64_t hog_engine_v1_00_a_controller_config::GetResultAddress()
{
	return m_ControllerConfig.ResultAddress;
}

void hog_engine_v1_00_a_controller_config::SetResultAddress(uint64_t value)
{
    m_ControllerConfig.ResultAddress = value;
}

VortexEntityType hog_engine_v1_00_a_controller_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_controller_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t hog_engine_v1_00_a_controller_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_controller_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

uint16_t hog_engine_v1_00_a_controller_config::GetNumChannels()
{
	switch(GetFormatColor())
	{
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::NONE : return 1; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::GRAY : return 1; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::RGB	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::RGBA : return 4; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::BGR	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::BGRA : return 4; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::HSV	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::HLS	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::YCC	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::LAB	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::LUV	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::XYZ	 : return 3; break;
	default : return 1; break;
	}
}

uint16_t hog_engine_v1_00_a_controller_config::GetNumChannelsValid()
{
    switch(GetFormatColor())
	{
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::NONE : return 1; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::GRAY : return 1; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::RGB	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::RGBA : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::BGR	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::BGRA : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::HSV	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::HLS	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::YCC	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::LAB	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::LUV	 : return 3; break;
	case IMAGE_DESCRIPTOR_FORMAT_COLOR::XYZ	 : return 3; break;
	default : return 1; break;
	}
}

uint64_t hog_engine_v1_00_a_controller_config::GetImageSizeBytes()
{
	switch(GetFormatDepth())
	{
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE :
		return GetWidth() * GetHeight() * GetNumChannels() * 1;
		break;
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::INT16 :
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT16:
		return GetWidth() * GetHeight() * GetNumChannels() * 2;
		break;
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::INT32:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT32:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::FLOAT:
		return GetWidth() * GetHeight() * GetNumChannels() * 4;
		break;
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::DOUBLE:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::INT64:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT64:
		return GetWidth() * GetHeight() * GetNumChannels() * 8;
		break;
	default:
		return 0;
	}
}

/*
uint32_t hog_engine_v1_00_a_controller_config::GetIPThreshold()
{
	return m_ControllerConfig.IPThreshold;
}

void hog_engine_v1_00_a_controller_config::SetIPThreshold(uint32_t value)
{
	m_ControllerConfig.IPThreshold = value;
}
*/

VortexConfigurationPacket* hog_engine_v1_00_a_controller_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_ControllerConfig.GetBytes(),16);

	return m_Vcp;
}
