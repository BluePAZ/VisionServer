#ifndef __HOG_ENGINE_V1_00_A_CONVOLUTION_KERNEL_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_CONVOLUTION_KERNEL_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _ConvolutionKernelConfig {
		
	//@ I am not using opcode in the current version of the hardware - ska - 10/19/2015
	// Convolution Kernel configurations:
	// Configuration Packet
	// We have a 128 bit configuration vector. We pack 16 bit coefficients in the following way
	// -------------------------------------------------------------------------------------------------------
	// |  0  | KERNEL [1][0]    | .......  | KERNEL [0][2]    |  0   | KERNEL [0][1]    |  0   |KERNEL [0][0]  |
	// -------------------------------------------------------------------------------------------------------
	//  127  |113             98|        80|79              64|63  48|47              32|31  16|15             0

	vector<vector<int16_t>> m_Kernels;
	
	// Since most kernels have at the most 9 coefficients we will pack 4 coefficients into each 16 byte config vector
	vector<uint8_t> Bytes;

	vector<uint8_t> GetBytes()
    {
		for (int i = 0; i < m_Kernels.size(); i++)
		{
			for (int j = 0; j < m_Kernels[i].size(); j++)
			{
				Bytes.push_back((uint16_t)(m_Kernels[i][j]) & 0x00FF); // First byte of 16 bit coefficient
				Bytes.push_back(((uint16_t)m_Kernels[i][j] & 0xFF00) >> 8); // Second byte of 16 bit coefficient
				Bytes.push_back(0); // 1 byte space
				Bytes.push_back(0); // 1 byte space
			}
		}

		return Bytes;
    }
} ConvolutionKernelConfig;

class hog_engine_v1_00_a_convolution_kernel_config : public IVortexConfiguration
{
public:
	hog_engine_v1_00_a_convolution_kernel_config(void);
	~hog_engine_v1_00_a_convolution_kernel_config(void);

	//Kernel
	vector<vector<int16_t>> GetKernel();
	void SetKernel(int16_t* values);
	int16_t* CreateDerivativeFilters();

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();
	uint8_t* GetBytes();

private:
	int16_t der_Kernel[3][3];  // 2d array
	VortexConfigurationPacket* m_Vcp;
	ConvolutionKernelConfig m_ConvolutionKernelConfig;
	uint8_t* m_coeffs;
};

#endif // !1

