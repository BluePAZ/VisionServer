#ifndef __GABOR_EXTRACTOR_V1_00_A_H__
#define __GABOR_EXTRACTOR_V1_00_A_H__


#include <vector>
#include <stdint.h>
#include "VortexTypes.h"

using namespace std;

class gabor_extractor_v1_00_a
{
public:
	gabor_extractor_v1_00_a(void);
	gabor_extractor_v1_00_a(uint64_t BaseAddress);
	~gabor_extractor_v1_00_a(void);

	vector<VortexConfigurationPacket*> GetConfiguration(int ImageWidth, int ImageHeight);
	vector<VortexConfigurationPacket*> GetConfiguration(vector<int> ImageWidths, vector<int> ImageHeights);
	vector<vector<vector<int16_t> > > GetKernels();
	vector<vector<int> > GetMinMax();
	//vector<vector<int64_t> > Convolve(vector<vector<vector<uint8_t> > > Image, int ImageChannel);
	void WriteKernelSimulationFile(string Filename, VortexConfigurationPacket* KernelConfiguration, VortexConfigurationPacket* ImageConfiguration);
	void WriteOutputSimulationFile(string Filename, int ResultBitwidth, vector<vector<int64_t> > Result);

private:
    static const int m_KernelNumFractionalBits = 14;

	uint64_t m_ConfigBaseAddress[2];
	uint64_t m_BaseAddress;
	int m_MaxKernelWidth;
	int m_MaxKernelHeight;
	vector<vector<vector<int16_t> > > m_Kernels;
    void CreateGaborFilterBank();

    vector<int> GetMinMax(vector<vector<int16_t> > KernelValues);
    vector<vector<int16_t> > CreateGaborFilter(double Angle, bool Odd, double Wavelength, double Sigma, int KernelSize, double Gamma);
	VortexConfigurationPacket* GetImageConfiguration(vector<int> ImageWidths, vector<int> ImageHeights);
};
#endif

