#include "hog_engine_v1_00_a_convolution_kernel_config.h"


hog_engine_v1_00_a_convolution_kernel_config::hog_engine_v1_00_a_convolution_kernel_config(void)
{
	der_Kernel[3][3] = {0};
	//CreateDerivativeFilters();
	
	m_Vcp = new VortexConfigurationPacket();


}

hog_engine_v1_00_a_convolution_kernel_config::~hog_engine_v1_00_a_convolution_kernel_config(void)
{
	delete(m_Vcp);
	delete[] m_coeffs;
}

int16_t* hog_engine_v1_00_a_convolution_kernel_config::CreateDerivativeFilters()
{
	der_Kernel[0][0] = 0;
	der_Kernel[0][1] = -1;
	der_Kernel[0][2] = 0;

	der_Kernel[1][0] = -1;
	der_Kernel[1][1] = 0;
	der_Kernel[1][2] = 1;

	der_Kernel[2][0] = 0;
	der_Kernel[2][1] = 1;
	der_Kernel[2][2] = 0;

	int16_t(*k)[3] = der_Kernel;

	return (int16_t*)k;
}

/*
uint8_t hog_engine_v1_00_a_convolution_kernel_config::GetOpcode()
{
	return m_ConvolutionKernelConfig.Opcode;
}

void hog_engine_v1_00_a_convolution_kernel_config::SetOpcode(uint8_t value)
{
	m_ConvolutionKernelConfig.Opcode = value;
}

uint16_t hog_engine_v1_00_a_convolution_kernel_config::GetWidth()
{
	return m_ConvolutionKernelConfig.Width;
}

void hog_engine_v1_00_a_convolution_kernel_config::SetMaxKernelWidth(uint8_t value)
{
    m_ConvolutionKernelConfig.Width = value;
}

uint16_t hog_engine_v1_00_a_convolution_kernel_config::GetMaxKernelHeight()
{
    return m_ConvolutionKernelConfig.maxKernelHeight;
}

void hog_engine_v1_00_a_convolution_kernel_config::SetMaxKernelHeight(uint8_t value)
{
    m_ConvolutionKernelConfig.maxKernelHeight = value;
}
*/

vector<vector<int16_t>> hog_engine_v1_00_a_convolution_kernel_config::GetKernel()
{
	return m_ConvolutionKernelConfig.m_Kernels;
}

void hog_engine_v1_00_a_convolution_kernel_config::SetKernel(int16_t* values)
{
	// Clearing before populating else results in accumulating over time - ska - 11/21/2015
	m_ConvolutionKernelConfig.m_Kernels.clear();

	int offset = 3;

	for (int i = 0; i < 3; i++)
	{
		vector<int16_t> temp;
		temp.push_back(*(values + offset*i + 0));
		temp.push_back(*(values + offset*i + 1));
		temp.push_back(*(values + offset*i + 2));

		m_ConvolutionKernelConfig.m_Kernels.push_back(temp);
	}

}

uint64_t hog_engine_v1_00_a_convolution_kernel_config::GetIndex()
{
	return GetConfiguration()->GetAddress();
}

void hog_engine_v1_00_a_convolution_kernel_config::SetIndex(uint64_t Index)
{
	GetConfiguration()->SetAddress(Index);
}

VortexConfigurationPacket* hog_engine_v1_00_a_convolution_kernel_config::GetConfiguration()
{
	m_Vcp->Clear();

	vector<uint8_t> kerneldata = m_ConvolutionKernelConfig.GetBytes();

	for (int i = 0; i <kerneldata.size(); i++)
	{
		uint8_t kernelcoeff = kerneldata[i];
		m_Vcp->Add(kernelcoeff);
	}
	return m_Vcp;
}

VortexEntityType hog_engine_v1_00_a_convolution_kernel_config::GetConfigurationType()
{
	return VortexEntityType::SAP;
}

void hog_engine_v1_00_a_convolution_kernel_config::SetConfigurationType(VortexEntityType Type)
{
}

// Adding this so as to send kernel coefficients via message interface
uint8_t* hog_engine_v1_00_a_convolution_kernel_config::GetBytes()
{
	vector<uint8_t> coeffs = m_ConvolutionKernelConfig.GetBytes();

	int total_bytes = ((coeffs.size() / 16) + 1)*16;

	// Needs to be multiple of 16 bytes
	m_coeffs = new uint8_t[total_bytes];
	if (m_coeffs == NULL)
		printf("could not allocate memory for m_coeffs\n");
	else
	{
		for (int i = 0; i < total_bytes; i++)
		{
			m_coeffs[i] = 0;
		}
	}

	for (int i = 0; i < coeffs.size(); i++)
	{
		//printf("%d\n",coeffs.at(i));
		m_coeffs[i] = coeffs.at(i);
	}

	return m_coeffs;
}
