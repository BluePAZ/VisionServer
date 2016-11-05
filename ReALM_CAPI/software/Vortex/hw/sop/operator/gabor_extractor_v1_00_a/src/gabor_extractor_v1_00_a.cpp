#include "gabor_extractor_v1_00_a.h"
#define _USE_MATH_DEFINES // for C++
#include <cmath>


#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

gabor_extractor_v1_00_a::gabor_extractor_v1_00_a(void)
{
	m_ConfigBaseAddress[0] = 0x0000;
	m_ConfigBaseAddress[1] = 0x4000;
	m_BaseAddress = 0;
	m_MaxKernelWidth = 9;
	m_MaxKernelHeight = 9;
    CreateGaborFilterBank();
}

gabor_extractor_v1_00_a::gabor_extractor_v1_00_a(uint64_t BaseAddress)
{
	m_ConfigBaseAddress[0] = 0x0000;
	m_ConfigBaseAddress[1] = 0x4000;
	m_BaseAddress = BaseAddress;
	m_MaxKernelWidth = 9;
	m_MaxKernelHeight = 9;
    CreateGaborFilterBank();
}

gabor_extractor_v1_00_a::~gabor_extractor_v1_00_a(void)
{
}

void gabor_extractor_v1_00_a::CreateGaborFilterBank()
{
    int Scale = m_MaxKernelWidth;
    int NumOrientations = 6;

    m_Kernels.resize(NumOrientations);
    for (int o = 0; o < NumOrientations; o++)
    {
        double sigma = (0.0036 * pow(Scale, 2.0)) + 0.35 * Scale + 0.18;
        double wavelength = sigma / 0.8;
        double angle = ((double)o) * (M_PI / (double)NumOrientations);
        m_Kernels[o] = CreateGaborFilter(angle, true, wavelength, sigma, Scale, 3);
    }
}

vector<VortexConfigurationPacket*> gabor_extractor_v1_00_a::GetConfiguration(int ImageWidth, int ImageHeight)
{
	//int NumConfigs = Kernels.size();
	vector<int> ImageWidths(1);
	vector<int> ImageHeights(1);

	ImageWidths[0] = ImageWidth;
	ImageHeights[0] = ImageHeight;

	return GetConfiguration(ImageWidths,ImageHeights);
}

vector<VortexConfigurationPacket*> gabor_extractor_v1_00_a::GetConfiguration(vector<int> ImageWidths, vector<int> ImageHeights)
{
	vector<VortexConfigurationPacket*> vcp(1);
	//vcp[0] = GetKernelConfiguration(Kernels);
	vcp[0] = GetImageConfiguration(ImageWidths,ImageHeights);

	return vcp;
}

vector<vector<vector<int16_t>>> gabor_extractor_v1_00_a::GetKernels()
{
    return m_Kernels;
}

VortexConfigurationPacket* gabor_extractor_v1_00_a::GetImageConfiguration(vector<int> ImageWidths, vector<int> ImageHeights)
{
	vector<uint8_t> ImageConfigBytes;

	uint16_t ImageWidth  = ImageWidths[0];
	uint16_t ImageHeight = ImageHeights[0];

	ImageConfigBytes.push_back(((uint8_t*)&ImageWidth)[0]);
	ImageConfigBytes.push_back(((uint8_t*)&ImageWidth)[1]);
	ImageConfigBytes.push_back(((uint8_t*)&ImageHeight)[0]);
	ImageConfigBytes.push_back(((uint8_t*)&ImageHeight)[1]);
	ImageConfigBytes.push_back((uint8_t)m_Kernels[0].size());
	ImageConfigBytes.push_back((uint8_t)m_Kernels[0][0].size());
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);
	ImageConfigBytes.push_back(0);

	return new VortexConfigurationPacket(ImageConfigBytes.data(), ImageConfigBytes.size(), m_ConfigBaseAddress[0] + m_BaseAddress);
}

vector<vector<int16_t>> gabor_extractor_v1_00_a::CreateGaborFilter(double Angle, bool Odd, double Wavelength, double Sigma, int KernelSize, double Gamma)
{
	int center = (KernelSize - 1) / 2;
	vector<vector<double>> u1(KernelSize);
	vector<vector<double>> u2(KernelSize);
	vector<vector<double>> gaussian(KernelSize);
	vector<vector<double>> wave(KernelSize);
	vector<vector<double>> gabor(KernelSize);
	vector<vector<int16_t>> gaborFxd(KernelSize);

	for (int i=0; i<KernelSize; i++)
	{
		u1[i].resize(KernelSize);
		u2[i].resize(KernelSize);
		gaussian[i].resize(KernelSize);
		wave[i].resize(KernelSize);
		gabor[i].resize(KernelSize);
		gaborFxd[i].resize(KernelSize);
	}

	double normX = cos(Angle);
    double normY = sin(Angle);

    double gamma_sqr = Gamma * Gamma;
    double sigma_sqr = Sigma * Sigma;
    double half_kernelsize_sqr = (((double)KernelSize) / 2) * (((double)KernelSize) / 2);

    double energy = 0;

	for (int row = (0 - center); row < KernelSize - center; row++)
    {
        for (int col = (0 - center); col < KernelSize - center; col++)
        {
            double u1e = u1[row + center][col + center] = row * normX + col * normY;
            double u2e = u2[row + center][col + center] = -row * normY + row * normX;
            gaussian[row + center][col + center] = exp(-(u1e * u1e + gamma_sqr * (u2e * u2e)) / (2 * sigma_sqr));
            if (Odd)
                wave[row + center][col + center] = cos(2 * M_PI / Wavelength * u1e - M_PI / 2);
            else
                wave[row + center][col + center] = cos(2 * M_PI / Wavelength * u1e);

            gabor[row + center][col + center] = gaussian[row + center][col + center] * wave[row + center][col + center];
            double mag = ((double)row * (double)row) + ((double)col * (double)col);
            gabor[row + center][col + center] = (mag > half_kernelsize_sqr) ? 0 : gabor[row + center][col + center];
            energy += pow(gabor[row + center][col + center], 2.0);
        }
    }

	energy = sqrt(energy);


    for (int row = 0; row < gabor.size(); row++)
    {
        for (int col = 0; col < gabor[0].size(); col++)
        {
            int16_t Number = gaborFxd[row][col] = (int16_t)((gabor[row][col] /= energy) * pow(2, m_KernelNumFractionalBits));
        }
    }

    return gaborFxd;
}

vector<vector<int>> gabor_extractor_v1_00_a::GetMinMax()
{
	int NumOrientations = m_Kernels.size();

    vector<vector<int>> MinMax(NumOrientations);

	for (int i = 0; i < NumOrientations; i++)
	{
		vector<int> tempMinMax = GetMinMax(m_Kernels[i]);
		MinMax[i].resize(3);
		MinMax[i][0] = i;
		MinMax[i][1] = tempMinMax[0];
		MinMax[i][2] = tempMinMax[1];
	}

	return MinMax;
}

vector<int> gabor_extractor_v1_00_a::GetMinMax(vector<vector<int16_t>> KernelValues)
{
    int MAX_PIXEL_VALUE = 255;
    int NumOrientations = KernelValues.size();

    vector<int> MinMax(2);

    int Height = KernelValues.size();
    int Width = KernelValues[0].size();

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            if (KernelValues[y][x] < 0)
                MinMax[0] += KernelValues[y][x] * MAX_PIXEL_VALUE;
            else
                MinMax[1] += KernelValues[y][x] * MAX_PIXEL_VALUE;
        }
    }

    return MinMax;
}
