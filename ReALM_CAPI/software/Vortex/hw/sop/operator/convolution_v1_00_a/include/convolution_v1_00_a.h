#pragma once

#include <vector>
#include <stdint.h>
#include "VortexTypes.h"

using namespace std;

class convolution_v1_00_a
{
public:
	convolution_v1_00_a(void);
	convolution_v1_00_a(uint64_t BaseAddress, int MaxKernelWidth, int MaxKernelHeight);
	~convolution_v1_00_a(void);

	vector<VortexConfigurationPacket*> GetConfiguration(vector<vector<vector<int16_t>>> Kernels, int ImageWidth, int ImageHeight);
	vector<VortexConfigurationPacket*> GetConfiguration(vector<vector<vector<int16_t>>> Kernels, vector<int> ImageWidths, vector<int> ImageHeights);
	vector<vector<int64_t>> Convolve(vector<vector<vector<uint8_t>>> Image, vector<vector<int16_t>> Kernel, int ImageChannel);
	void WriteKernelSimulationFile(string Filename, VortexConfigurationPacket* KernelConfiguration, VortexConfigurationPacket* ImageConfiguration);
	void WriteOutputSimulationFile(string Filename, int ResultBitwidth, vector<vector<int64_t>> Result);
private:
	uint64_t m_ConfigBaseAddress[2];
	uint64_t m_BaseAddress;
	int m_MaxKernelWidth;
	int m_MaxKernelHeight;
	int m_MaxNumKernels;
	vector<vector<vector<int16_t>>> m_Kernels;

	VortexConfigurationPacket* GetImageConfiguration(vector<vector<vector<int16_t>>> Kernels, vector<int> ImageWidths, vector<int> ImageHeights);
	VortexConfigurationPacket* GetKernelConfiguration(vector<vector<vector<int16_t>>> Kernels);
	vector<uint8_t> GetKernelBytes();
};

