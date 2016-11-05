#include "AIMProcessorConfig.h"


AIMProcessorConfig::AIMProcessorConfig(void)
{
    m_ProcessorConfig.FlowID = 0;
	m_AccumDeviceID = NULL;
	m_Vcp = new VortexConfigurationPacket();
}

AIMProcessorConfig::~AIMProcessorConfig(void)
{
	delete(m_AccumDeviceID);
	delete(m_Vcp);
}

uint16_t AIMProcessorConfig::GetWidth()
{
	return m_ProcessorConfig.Width;
}

void AIMProcessorConfig::SetWidth(uint16_t value)
{
    m_ProcessorConfig.Width = value;
}

uint16_t AIMProcessorConfig::GetHeight()
{
    return m_ProcessorConfig.Height;
}

void AIMProcessorConfig::SetHeight(uint16_t value)
{
    m_ProcessorConfig.Height = value;
}

IMAGE_DESCRIPTOR_TYPE AIMProcessorConfig::GetType()
{
	return (IMAGE_DESCRIPTOR_TYPE)m_ProcessorConfig.Type;
}

void AIMProcessorConfig::SetType(IMAGE_DESCRIPTOR_TYPE value)
{
    m_ProcessorConfig.Type = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_COLOR AIMProcessorConfig::GetFormatColor()
{
	return (IMAGE_DESCRIPTOR_FORMAT_COLOR)m_ProcessorConfig.Color;
}

void AIMProcessorConfig::SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value)
{
    m_ProcessorConfig.Color = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_DEPTH AIMProcessorConfig::GetFormatDepth()
{
	return (IMAGE_DESCRIPTOR_FORMAT_DEPTH)m_ProcessorConfig.Depth;
}

void AIMProcessorConfig::SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value)
{
    m_ProcessorConfig.Depth = (uint8_t)value;
}

void AIMProcessorConfig::SetBaseFlowID(uint16_t value)
{
    m_ProcessorConfig.FlowID = value;
}

uint16_t AIMProcessorConfig::GetBaseFlowID()
{
    return m_ProcessorConfig.FlowID;
}

uint16_t AIMProcessorConfig::GetPixelFetchFlowID()
{
	return m_ProcessorConfig.FlowID;
}

uint16_t AIMProcessorConfig::GetAccumulatorFetchFlowID()
{
	return (m_ProcessorConfig.FlowID == 0) ? 0 : m_ProcessorConfig.FlowID + 1;
}

uint16_t AIMProcessorConfig::GetAccumulatorStoreFlowID()
{
	return (m_ProcessorConfig.FlowID == 0) ? 0 : m_ProcessorConfig.FlowID + 2;
}

VortexDeviceAddress* AIMProcessorConfig::GetResultDeviceID()
{
	return m_AccumDeviceID;
}

void AIMProcessorConfig::SetResultDeviceID(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	delete(m_AccumDeviceID);
	m_AccumDeviceID = new VortexDeviceAddress(BusID,SwitchID,PortID);
	m_ProcessorConfig.AccumDeviceID = m_AccumDeviceID->GetDeviceID();
}

uint64_t AIMProcessorConfig::GetResultAddress()
{
	return m_ProcessorConfig.AccumAddress;
}

void AIMProcessorConfig::SetResultAddress(uint64_t value)
{
    m_ProcessorConfig.AccumAddress = value;
}

VortexEntityType AIMProcessorConfig::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void AIMProcessorConfig::SetConfigurationType(VortexEntityType ConfigType)
{
}

uint64_t AIMProcessorConfig::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void AIMProcessorConfig::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

uint16_t AIMProcessorConfig::GetNumChannels()
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

uint16_t AIMProcessorConfig::GetNumChannelsValid()
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

uint64_t AIMProcessorConfig::GetImageSizeBytes()
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

VortexConfigurationPacket* AIMProcessorConfig::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_ProcessorConfig.GetBytes(),16);

	return m_Vcp;
}
