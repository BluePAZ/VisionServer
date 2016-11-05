#include "VortexImageDescriptor.h"

VortexImageDescriptor::VortexImageDescriptor(void)
{
}

VortexImageDescriptor::VortexImageDescriptor(uint8_t* value)
{
    for (int i=0; i<16; i++)
        m_Descriptor.Bytes[i] = value[i];
}


VortexImageDescriptor::~VortexImageDescriptor(void)
{
}

VortexMemoryAllocation* VortexImageDescriptor::GetImageHandle()
{
	m_ImageHandle->SetMemoryDeviceID(m_Descriptor.DeviceID);
	m_ImageHandle->SetMemoryOffset(m_Descriptor.Address);

	return m_ImageHandle;
}

void VortexImageDescriptor::SetImageHandle(VortexMemoryAllocation* value)
{
    m_Descriptor.DeviceID = value->GetMemoryDeviceID()->GetDeviceID();
    m_Descriptor.Address = value->GetMemoryOffset();
}

uint16_t VortexImageDescriptor::GetIdentifier()
{
    return m_Descriptor.Identifier;
}

void VortexImageDescriptor::SetIdentifier(uint16_t value)
{
    m_Descriptor.Identifier = value;
}

uint16_t VortexImageDescriptor::GetWidth()
{
    return m_Descriptor.Width;
}

void VortexImageDescriptor::SetWidth(uint16_t value)
{
    m_Descriptor.Width = value;
}

uint16_t VortexImageDescriptor::GetHeight()
{
    return m_Descriptor.Height;
}

void VortexImageDescriptor::SetHeight(uint16_t value)
{
    m_Descriptor.Height = value;
}

IMAGE_DESCRIPTOR_TYPE VortexImageDescriptor::GetType()
{
    return (IMAGE_DESCRIPTOR_TYPE)m_Descriptor.Type;
}

void VortexImageDescriptor::SetType(IMAGE_DESCRIPTOR_TYPE value)
{
    m_Descriptor.Type = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_COLOR VortexImageDescriptor::GetFormatColor()
{
    return (IMAGE_DESCRIPTOR_FORMAT_COLOR)m_Descriptor.Color;
}

void VortexImageDescriptor::SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value)
{
    m_Descriptor.Color = (uint8_t)value;
}

IMAGE_DESCRIPTOR_FORMAT_DEPTH VortexImageDescriptor::GetFormatDepth()
{
	return (IMAGE_DESCRIPTOR_FORMAT_DEPTH)m_Descriptor.Depth;
}

void VortexImageDescriptor::SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value)
{
    m_Descriptor.Depth = (uint8_t)value;
}

uint8_t* VortexImageDescriptor::GetBytes(int& Length)
{
	Length = 16;
	return m_Descriptor.GetBytes();
}

uint8_t* VortexImageDescriptor::GetBytes()
{
	return m_Descriptor.GetBytes();
}

int VortexImageDescriptor::GetImageSize()
{
	int bytesPerPixel = 0;
	switch (GetFormatDepth())
	{
	case  IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE :
		bytesPerPixel = 1;
		break;
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::INT16 :
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT16:
		bytesPerPixel = 2;
		break;
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::FLOAT:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::INT32:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT32:
		bytesPerPixel = 4;
		break;
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::DOUBLE:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::INT64:
	case IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT64:
		bytesPerPixel = 8;
		break;
	}

	return GetWidth() * GetHeight() * bytesPerPixel;
}
