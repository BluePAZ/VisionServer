#include "aim_engine_v2_00_b_processor_config.h"


aim_processor_config::aim_processor_config(void)
{
    m_ProcessorConfig.FlowID = 0;
	m_AccumDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

aim_processor_config::~aim_processor_config(void)
{
	delete(m_AccumDeviceID);
	delete(m_Vcp);
}

uint16_t aim_processor_config::GetWidth()
{
	return m_ProcessorConfig.Width;
}

void aim_processor_config::SetWidth(uint16_t value)
{
    m_ProcessorConfig.Width = value;
}

uint16_t aim_processor_config::GetHeight()
{
    return m_ProcessorConfig.Height;
}

void aim_processor_config::SetHeight(uint16_t value)
{
    m_ProcessorConfig.Height = value;
}

IMAGE_DESCRIPTOR_TYPE aim_processor_config::GetType()
{
	return (IMAGE_DESCRIPTOR_TYPE)m_ProcessorConfig.Type;
}

void aim_processor_config::SetType(IMAGE_DESCRIPTOR_TYPE value)
{
    m_ProcessorConfig.Type = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_COLOR aim_processor_config::GetFormatColor()
{
	return (IMAGE_DESCRIPTOR_FORMAT_COLOR)m_ProcessorConfig.Color;
}

void aim_processor_config::SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value)
{
    m_ProcessorConfig.Color = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_DEPTH aim_processor_config::GetFormatDepth()
{
	return (IMAGE_DESCRIPTOR_FORMAT_DEPTH)m_ProcessorConfig.Depth;
}

void aim_processor_config::SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value)
{
    m_ProcessorConfig.Depth = (uint8_t)value;
}

void aim_processor_config::SetBaseFlowID(uint16_t value)
{
    m_ProcessorConfig.FlowID = value;
}

uint16_t aim_processor_config::GetBaseFlowID()
{
    return m_ProcessorConfig.FlowID;
}

uint16_t aim_processor_config::GetPixelFetchFlowID()
{
	return m_ProcessorConfig.FlowID;
}

uint16_t aim_processor_config::GetAccumulatorFetchFlowID()
{
	return (m_ProcessorConfig.FlowID == 0) ? 0 : m_ProcessorConfig.FlowID + 1;
}

uint16_t aim_processor_config::GetAccumulatorStoreFlowID()
{
	return (m_ProcessorConfig.FlowID == 0) ? 0 : m_ProcessorConfig.FlowID + 2;
}

VortexDeviceAddress* aim_processor_config::GetResultDeviceID()
{
	return m_AccumDeviceID;
}

void aim_processor_config::SetResultDeviceID(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	delete(m_AccumDeviceID);
	m_AccumDeviceID = new VortexDeviceAddress(BusID,SwitchID,PortID);
	m_ProcessorConfig.AccumDeviceID = m_AccumDeviceID->GetDeviceID();
}

uint64_t aim_processor_config::GetResultAddress()
{
	return m_ProcessorConfig.AccumAddress;
}

void aim_processor_config::SetResultAddress(uint64_t value)
{
    m_ProcessorConfig.AccumAddress = value;
}

VortexEntityType aim_processor_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void aim_processor_config::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t aim_processor_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void aim_processor_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

uint16_t aim_processor_config::GetNumChannels()
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

uint16_t aim_processor_config::GetNumChannelsValid()
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

uint64_t aim_processor_config::GetImageSizeBytes()
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

VortexConfigurationPacket* aim_processor_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_ProcessorConfig.GetBytes(),16);

	return m_Vcp;
}
