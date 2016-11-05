#include "block_norm_v1_00_a_image_configuration.h"


block_norm_v1_00_a_image_configuration::block_norm_v1_00_a_image_configuration(void)
{
}

block_norm_v1_00_a_image_configuration::~block_norm_v1_00_a_image_configuration(void)
{
}

uint16_t block_norm_v1_00_a_image_configuration::GetWidth()
{
	return m_config.Width;
}

void block_norm_v1_00_a_image_configuration::SetWidth(uint16_t value)
{
    m_config.Width = value;
}

uint16_t block_norm_v1_00_a_image_configuration::GetHeight()
{
    return m_config.Height;
}

void block_norm_v1_00_a_image_configuration::SetHeight(uint16_t value)
{
    m_config.Height = value;
}

IMAGE_DESCRIPTOR_TYPE block_norm_v1_00_a_image_configuration::GetType()
{
	return (IMAGE_DESCRIPTOR_TYPE)m_config.Type;
}

void block_norm_v1_00_a_image_configuration::SetType(IMAGE_DESCRIPTOR_TYPE value)
{
    m_config.Type = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_COLOR block_norm_v1_00_a_image_configuration::GetFormatColor()
{
	return (IMAGE_DESCRIPTOR_FORMAT_COLOR)m_config.Color;
}

void block_norm_v1_00_a_image_configuration::SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value)
{
    m_config.Color = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_DEPTH block_norm_v1_00_a_image_configuration::GetFormatDepth()
{
	return (IMAGE_DESCRIPTOR_FORMAT_DEPTH)m_config.Depth;
}

void block_norm_v1_00_a_image_configuration::SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value)
{
    m_config.Depth = (uint8_t)value;
}


uint16_t block_norm_v1_00_a_image_configuration::GetNumChannels()
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

uint16_t block_norm_v1_00_a_image_configuration::GetNumChannelsValid()
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

uint64_t block_norm_v1_00_a_image_configuration::GetImageSizeBytes()
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


