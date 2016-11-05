#include "histogram_v3_00_a.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>

double histogram_v3_00_a::log2(double X)
{
	return log(X) / log(2.0);
}

histogram_v3_00_a::histogram_v3_00_a(uint64_t BaseAddress, int NumHistograms, int NumHistogramBins)
{
	m_NumHistograms = NumHistograms;
	m_NumHistogramBins = NumHistogramBins;
	m_BaseAddress = BaseAddress;

	m_MinMaxConfigBaseAddress = BaseAddress + 0x000000;
    m_PartitionConfigBaseAddress = BaseAddress + 0x100000;
    m_ImageSizeConfigBaseAddress = BaseAddress + 0x200000;
}

histogram_v3_00_a::~histogram_v3_00_a(void)
{
}

int histogram_v3_00_a::GetNumHistogramBins()
{
	return m_NumHistogramBins;
}

int histogram_v3_00_a::GetHistogramIndexWidth()
{
	return ceil(log2(m_NumHistogramBins) + log2(m_NumHistograms));
}

void histogram_v3_00_a::SetPartition(HistogramPartition* Partitions)
{
	m_Partitions = Partitions;
}

void histogram_v3_00_a::Init()
{
	m_Bins.clear();
	m_Bins.resize(m_NumHistograms);

	for (std::vector<vector<int>>::iterator it = m_Bins.begin() ; it != m_Bins.end(); ++it)
		(*it).resize(m_NumHistogramBins);
}

vector<VortexConfigurationPacket*> histogram_v3_00_a::GetConfiguration(vector<vector<int>> MinMaxConfig, int ImageWidth, int ImageHeight, HistogramPartition* Partitions)
{
	SetPartition(Partitions);

	vector<uint8_t> MinMaxConfigurationBytes = GetMinMaxConfiguration(MinMaxConfig);
	vector<uint8_t> PartitionConfigurationBytes = GetPartitionConfiguration();
	vector<uint8_t> ImageSizeConfigurationBytes = GetImageSizeConfiguration(ImageWidth,ImageHeight);

	VortexConfigurationPacket* vcp[3];
	vcp[0] = new VortexConfigurationPacket(MinMaxConfigurationBytes.data(),MinMaxConfigurationBytes.size(), m_MinMaxConfigBaseAddress);
	vcp[1] = new VortexConfigurationPacket(PartitionConfigurationBytes.data(), PartitionConfigurationBytes.size(), m_PartitionConfigBaseAddress);
	vcp[2] = new VortexConfigurationPacket(ImageSizeConfigurationBytes.data(), ImageSizeConfigurationBytes.size(), m_ImageSizeConfigBaseAddress);

	return vector<VortexConfigurationPacket*>(vcp,vcp + sizeof(vcp) / sizeof(vcp[0]));
}

vector<uint8_t> histogram_v3_00_a::GetImageSizeConfiguration(int ImageWidth, int ImageHeight)
{
	vector<uint8_t> ImageSizeConfigBytes;
	ImageSizeConfigBytes.push_back(((uint8_t*)&ImageWidth)[0]);
	ImageSizeConfigBytes.push_back(((uint8_t*)&ImageWidth)[1]);
	ImageSizeConfigBytes.push_back(((uint8_t*)&ImageHeight)[0]);
	ImageSizeConfigBytes.push_back(((uint8_t*)&ImageHeight)[1]);

	for (int i=0; i<(16-ImageSizeConfigBytes.size()); i++)
		ImageSizeConfigBytes.push_back(0);

	return ImageSizeConfigBytes;
}

vector<uint8_t> histogram_v3_00_a::GetMinMaxConfiguration(vector<vector<int>> MinMaxConfig)
{
	vector<uint8_t> MinMaxConfigBytes;

	for (int i=0; i<MinMaxConfig.size(); i++)
	{
		MinMaxConfigBytes.push_back((uint8_t)(MinMaxConfig[i][0] & 0xFF));
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][1] >> 0 ) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][1] >> 8 ) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][1] >> 16) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][1] >> 24) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][2] >> 0 ) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][2] >> 8 ) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][2] >> 16) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][2] >> 24) & 0xFF));
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
	}

	return MinMaxConfigBytes;
}

vector<uint8_t> histogram_v3_00_a::GetPartitionConfiguration()
{
	vector<uint16_t> Mappings = m_Partitions->GetMappings();
	uint16_t* MappingsArray = Mappings.data();
	vector<uint8_t> MappingConfigBytes(Mappings.size()*16);

	for (int i=0; i<Mappings.size(); i++)
	{
		MappingConfigBytes[(i*16) + 0] = ((uint8_t)((MappingsArray[i] >> 0) & 0xFF));
		MappingConfigBytes[(i*16) + 1] = ((uint8_t)((MappingsArray[i] >> 8) & 0xFF));
		for (int k = 2; k < 16; k++)
			MappingConfigBytes[(i*16) + k] = 0;
	}

	return MappingConfigBytes;
}

vector<int64_t> histogram_v3_00_a::BuildHistogram(vector<vector<int64_t>> Image, int RangeMin, int RangeMax)
{
	Init();

	vector<int64_t> Indices;
	int index_num_bits = (int)log2(m_NumHistogramBins);
	int64_t index_mask = m_NumHistogramBins - 1;

	int64_t range_width = (((int64_t)RangeMax - (int64_t)RangeMin) >> (int)log2((double)m_NumHistogramBins));
	int clog2_range_width = (int)ceil(log2((double)range_width));

	int Height = Image.size();
	int Width = Image[0].size();

	for (int y = 0; y < Height; y++)
     {
         for (int x = 0; x < Width; x++)
         {
             int64_t index = 0;
             if (Image[y][x] > RangeMax)
                 index = m_NumHistogramBins - 1;
             else if (Image[y][x] < RangeMin)
                 index = 0;
             else
                 index = (Image[y][x] - RangeMin) >> clog2_range_width;

			 int HistogramSelect = m_Partitions->GetMapping(y,x);
			 m_Bins[HistogramSelect][index]++;

			 int64_t effectiveIndex = (index & index_mask) | ((int64_t)HistogramSelect << index_num_bits);
			 Indices.push_back(effectiveIndex);
         }
     }

	return Indices;
}

vector<vector<int64_t>> histogram_v3_00_a::IndexHistogram(vector<vector<int64_t>> Image, int RangeMin, int RangeMax)
{
	int Height = Image.size();
	int Width = Image[0].size();

	vector<vector<int64_t>> BackProjectedImage(Height);

	int64_t range_width = (((int64_t)RangeMax - (int64_t)RangeMin) >> (int)log2((double)m_NumHistogramBins));
	int clog2_range_width = (int)ceil(log2((double)range_width));

	for (int y = 0; y < Height; y++)
    {
		BackProjectedImage[y].resize(Width);
        for (int x = 0; x < Width; x++)
        {
            if (Image[y][x] > RangeMax)
				BackProjectedImage[y][x] = m_Bins[m_Partitions->GetMapping(y,x)][m_NumHistogramBins - 1];
            else if (Image[y][x] < RangeMin)
                BackProjectedImage[y][x] = m_Bins[m_Partitions->GetMapping(y,x)][0];
            else
                BackProjectedImage[y][x] = m_Bins[m_Partitions->GetMapping(y,x)][(Image[y][x] - RangeMin) >> clog2_range_width];
        }
    }

	return BackProjectedImage;
}

vector<vector<int64_t>> histogram_v3_00_a::GenerateHistogramTestData(vector<vector<int64_t>> Image, vector<int> MinMax, string OutputFilepath)
{
	vector<vector<int64_t>> BackProjectedResult;

	ofstream* swIndexModeData = new ofstream(OutputFilepath + "HistogramIndexModeData.dat");
    ofstream* swBuildModeIndex = new ofstream(OutputFilepath + "HistogramBuildModeIndex.dat");
    ofstream* swIndexModeIndex = new ofstream(OutputFilepath + "HistogramIndexModeIndex.dat");

	vector<int64_t> HistogramBuildIndices = BuildHistogram(Image, MinMax[1], MinMax[2]);
	BackProjectedResult = IndexHistogram(Image, MinMax[1], MinMax[2]);
	WriteToFile(BackProjectedResult, 32, swIndexModeData);
	WriteToFile(HistogramBuildIndices, GetHistogramIndexWidth(), swBuildModeIndex);
    WriteToFile(HistogramBuildIndices, GetHistogramIndexWidth(), swIndexModeIndex);

	swIndexModeData->close();
	swBuildModeIndex->close();
	swIndexModeIndex->close();

	delete swIndexModeData;
	delete swBuildModeIndex;
	delete swIndexModeIndex;

	return BackProjectedResult;
}

void histogram_v3_00_a::WriteToFile(vector<vector<int64_t>> Image, int OutputDatawidth, ofstream* sw)
{
    int ImageHeight = Image.size();
    int ImageWidth = Image[0].size();

    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - OutputDatawidth);
    int formatWidth = (int)ceil((float)OutputDatawidth / 4);

    for (int y = 0; y < ImageHeight; y++)
    {
        for (int x = 0; x < ImageWidth; x++)
        {
            (*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Image[y][x] & Mask) << endl;
        }
    }
}

void histogram_v3_00_a::WriteToFile(vector<int64_t> Data, int OutputDatawidth, ofstream* sw)
{
    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - OutputDatawidth);
    int formatWidth = (int)ceil((float)OutputDatawidth / 4);

    for (int i = 0; i < Data.size(); i++)
    {
		(*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Data[i] & Mask) << endl;
    }
}
