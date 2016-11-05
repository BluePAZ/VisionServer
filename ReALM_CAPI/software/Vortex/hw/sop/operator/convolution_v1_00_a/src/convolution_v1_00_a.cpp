#include "convolution_v1_00_a.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

convolution_v1_00_a::convolution_v1_00_a(void)
{
	m_ConfigBaseAddress[0] = 0x0000;
	m_ConfigBaseAddress[1] = 0x4000;
	m_BaseAddress = 0;
	m_MaxKernelWidth = 15;
	m_MaxKernelHeight = 15;
	m_MaxNumKernels = 16;

	m_Kernels.resize(m_MaxNumKernels);
}

convolution_v1_00_a::convolution_v1_00_a(uint64_t BaseAddress, int MaxKernelWidth, int MaxKernelHeight)
{
	m_ConfigBaseAddress[0] = 0x0000;
	m_ConfigBaseAddress[1] = 0x4000;	
	m_BaseAddress = BaseAddress;
	m_MaxKernelWidth = MaxKernelWidth;
	m_MaxKernelHeight = MaxKernelHeight;
	m_MaxNumKernels = 16;
	m_Kernels.resize(m_MaxNumKernels);
}

convolution_v1_00_a::~convolution_v1_00_a(void)
{
}

vector<VortexConfigurationPacket*> convolution_v1_00_a::GetConfiguration(vector<vector<vector<int16_t>>> Kernels, int ImageWidth, int ImageHeight)
{
	int NumConfigs = Kernels.size();
	vector<int> ImageWidths(NumConfigs);
	vector<int> ImageHeights(NumConfigs);

	for (int i=0; i<NumConfigs; i++)
	{
		ImageWidths[i] = ImageWidth;
		ImageHeights[i] = ImageHeight;
	}

	return GetConfiguration(Kernels,ImageWidths,ImageHeights);
}

vector<VortexConfigurationPacket*> convolution_v1_00_a::GetConfiguration(vector<vector<vector<int16_t>>> Kernels, vector<int> ImageWidths, vector<int> ImageHeights)
{
	vector<VortexConfigurationPacket*> vcp(2);
	vcp[0] = GetKernelConfiguration(Kernels);
	vcp[1] = GetImageConfiguration(Kernels,ImageWidths,ImageHeights);

	return vcp;
}

VortexConfigurationPacket* convolution_v1_00_a::GetImageConfiguration(vector<vector<vector<int16_t>>> Kernels, vector<int> ImageWidths, vector<int> ImageHeights)
{
	vector<uint8_t> ImageConfigBytes;

	for (int i=0; i<Kernels.size(); i++)
	{
		uint16_t ImageWidth = 0;
		uint16_t ImageHeight = 0;

		if (ImageWidths.size() != ImageHeights.size())
			return NULL;

		if (ImageWidths.size() > i)
		{
			ImageWidth = ImageWidths[i];
			ImageHeight = ImageHeights[i];
		}

		ImageConfigBytes.push_back(((uint8_t*)&ImageWidth)[0]);
		ImageConfigBytes.push_back(((uint8_t*)&ImageWidth)[1]);
		ImageConfigBytes.push_back(((uint8_t*)&ImageHeight)[0]);
		ImageConfigBytes.push_back(((uint8_t*)&ImageHeight)[1]);
		ImageConfigBytes.push_back((uint8_t)Kernels[i].size());
		ImageConfigBytes.push_back((uint8_t)Kernels[i][0].size());
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
	}

	return new VortexConfigurationPacket(ImageConfigBytes.data(), ImageConfigBytes.size(), m_ConfigBaseAddress[0] + m_BaseAddress);
}

VortexConfigurationPacket* convolution_v1_00_a::GetKernelConfiguration(vector<vector<vector<int16_t>>> Kernels)
{
	for (int i = 0; i < m_Kernels.size(); i++)
	{
		m_Kernels[i].resize(m_MaxKernelHeight);
		for (int j = 0; j < m_MaxKernelHeight; j++)
		{
			m_Kernels[i][j].resize(m_MaxKernelWidth);
			for (int c=0; c<m_MaxKernelWidth; c++)
				m_Kernels[i][j][c] = 0;
		}

		if (i < Kernels.size())
		{
			for (int y = 0; y < min((int)Kernels[i].size(),m_MaxKernelHeight); y++)
			{
				for (int x = 0; x < min((int)Kernels[i][y].size(),m_MaxKernelWidth); x++)
				{
					m_Kernels[i][y][x] = Kernels[i][y][x];
				}
			}
		}
	}  

	vector<uint8_t> kernelBytes = GetKernelBytes();
	return new VortexConfigurationPacket(kernelBytes.data(), kernelBytes.size(), m_ConfigBaseAddress[1] + m_BaseAddress);
}

void convolution_v1_00_a::WriteKernelSimulationFile(string Filename, VortexConfigurationPacket* KernelConfiguration, VortexConfigurationPacket* ImageConfiguration)
{
	ofstream sw(Filename,std::ios_base::trunc);
	
	int ImageConfigLinesLength;
	string* ImageConfigLines = ImageConfiguration->GetConfigString(ImageConfigLinesLength);
			
	for (int i = 0; i < ImageConfigLinesLength; i++)
	{
		if (i == ImageConfigLinesLength - 1)
		{
			sw << string("1") << ImageConfigLines[i] << endl;
		}
		else
		{
			sw << string("0") << ImageConfigLines[i] << endl;
		}
	}

	int KernelConfigLinesLength;
	string* KernelConfigLines = KernelConfiguration->GetConfigString(KernelConfigLinesLength);

	for (int i = 0; i < KernelConfigLinesLength; i++)
	{
		if (i == KernelConfigLinesLength - 1)
		{
			sw << string("1") << KernelConfigLines[i] << endl;
		}
		else
		{
			sw << string("0") << KernelConfigLines[i] << endl;
		}
	}
	
	sw.close();
}

void convolution_v1_00_a::WriteOutputSimulationFile(string Filename, int ResultBitwidth, vector<vector<int64_t>> Result)
{
	int Height = Result.size();
	int Width = Result.size();

	ofstream sw(Filename,std::ios_base::trunc);

	uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - ResultBitwidth);

	int FormatWidth = ceil((float)ResultBitwidth / 4.0f);

	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
			sw << std::hex << std::setfill('0') << std::setw(FormatWidth) << ((uint64_t)Result[y][x] & Mask);

	sw.close();
}

vector<uint8_t> convolution_v1_00_a::GetKernelBytes()
{
	vector<uint8_t> KernelConfigBytes;

	for (int i = 0; i < m_Kernels.size(); i++)
    {
        for (int y = 0; y < m_MaxKernelHeight; y++)
        {
            for (int x = 0; x < m_MaxKernelWidth; x++)
            {
				uint16_t kernelData = m_Kernels[i][y][x];
				KernelConfigBytes.push_back(((uint8_t*)&kernelData)[0]);
				KernelConfigBytes.push_back(((uint8_t*)&kernelData)[1]);
                KernelConfigBytes.push_back(0);
                KernelConfigBytes.push_back(0);
            }
        }

        while ((KernelConfigBytes.size() % 1024) != 0)
            KernelConfigBytes.push_back(0);
    }

    int Residual = ((KernelConfigBytes.size() % 16) == 0) ? 0 : 16 - (KernelConfigBytes.size() % 16);

    for (int i = 0; i < Residual; i++)
        KernelConfigBytes.push_back(0);

	uint8_t* kernelBYtesPtr = KernelConfigBytes.data();

    return KernelConfigBytes;
}

vector<vector<int64_t>> convolution_v1_00_a::Convolve(vector<vector<vector<uint8_t>>> Image, vector<vector<int16_t>> Kernel, int ImageChannel)
{
	int imageHeight = Image.size();
	int imageWidth = Image[0].size();

	int kernelHeight = Kernel.size();
	int kernelWidth = Kernel[0].size();

	int start_row = (kernelHeight - 1) / 2;
    int start_col = (kernelWidth - 1) / 2;
    int end_row = imageHeight - ((kernelHeight - 1) / 2);
    int end_col = imageWidth - ((kernelWidth - 1) / 2);

	vector<vector<int64_t>> Result;

	for (int y = start_row; y < end_row; y++)
    {
        for (int x = start_col; x < end_col; x++)
        {
            for (int ky = 0; ky < kernelHeight; ky++)
            {
                for (int kx = 0; kx < kernelWidth; kx++)
                {
                    //if (Kernel[ky, kx] < 0)
                    //    Result[y, x] += Kernel[ky, kx] * Image[y - ((kernelHeight - 1) / 2) + ky, x - ((kernelWidth - 1) / 2) + kx, ImageChannel] * -1;
                    //else
                        Result[y][x] += Kernel[ky][kx] * Image[y - ((kernelHeight - 1) / 2) + ky][x - ((kernelWidth - 1) / 2) + kx][ImageChannel];
                }
            }
        }
    }

	return Result;
}
