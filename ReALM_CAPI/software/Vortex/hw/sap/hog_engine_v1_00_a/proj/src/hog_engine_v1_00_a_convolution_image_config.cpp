#include "hog_engine_v1_00_a_convolution_image_config.h"


hog_engine_v1_00_a_convolution_image_config::hog_engine_v1_00_a_convolution_image_config(void)
{
	//m_ConvolutionImageConfig.Opcode = 0;
	m_ConvolutionImageConfig.Width = 0;
	m_ConvolutionImageConfig.Height = 0;
	m_ConvolutionImageConfig.KernelWidth = 0;
	m_ConvolutionImageConfig.KernelHeight = 0;
	m_Vcp = new VortexConfigurationPacket();
}

hog_engine_v1_00_a_convolution_image_config::~hog_engine_v1_00_a_convolution_image_config(void)
{
	delete(m_Vcp);
}

/*
uint8_t hog_engine_v1_00_a_convolution_image_config::GetOpcode()
{
	return m_ConvolutionImageConfig.Opcode;
}

void hog_engine_v1_00_a_convolution_image_config::SetOpcode(uint8_t value)
{
	m_ConvolutionImageConfig.Opcode = value;
}
*/

uint16_t hog_engine_v1_00_a_convolution_image_config::GetWidth()
{
	return m_ConvolutionImageConfig.Width;
}

void hog_engine_v1_00_a_convolution_image_config::SetWidth(uint16_t value)
{
    m_ConvolutionImageConfig.Width = value;
}

uint16_t hog_engine_v1_00_a_convolution_image_config::GetHeight()
{
    return m_ConvolutionImageConfig.Height;
}

void hog_engine_v1_00_a_convolution_image_config::SetHeight(uint16_t value)
{
    m_ConvolutionImageConfig.Height = value;
}

uint8_t hog_engine_v1_00_a_convolution_image_config::GetKernelWidth()
{
	return m_ConvolutionImageConfig.KernelWidth;
}

void hog_engine_v1_00_a_convolution_image_config::SetKernelWidth(uint8_t value)
{
	m_ConvolutionImageConfig.KernelWidth = value;
}

uint8_t hog_engine_v1_00_a_convolution_image_config::GetKernelHeight()
{
	return m_ConvolutionImageConfig.KernelHeight;
}

void hog_engine_v1_00_a_convolution_image_config::SetKernelHeight(uint8_t value)
{
	m_ConvolutionImageConfig.KernelHeight = value;
}

uint64_t hog_engine_v1_00_a_convolution_image_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_convolution_image_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_convolution_image_config::GetConfiguration()
{
	m_Vcp->Clear();
	m_Vcp->Add(m_ConvolutionImageConfig.GetBytes(),16);

	return m_Vcp;
}

VortexEntityType hog_engine_v1_00_a_convolution_image_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_convolution_image_config::SetConfigurationType(VortexEntityType Type)
{
}
