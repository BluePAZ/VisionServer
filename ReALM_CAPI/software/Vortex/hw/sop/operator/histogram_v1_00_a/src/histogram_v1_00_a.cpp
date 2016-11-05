#include "histogram_v1_00_a.h"

double histogram_v1_00_a::log2(double X)
{
	return log(X) / log(2.0);
}

histogram_v1_00_a::histogram_v1_00_a(void)
{
}

histogram_v1_00_a::histogram_v1_00_a(uint64_t BaseAddress, int NumHistogramBins)
{
	m_NumHistogramBins = NumHistogramBins;
	m_BaseAddress = BaseAddress;
}

histogram_v1_00_a::~histogram_v1_00_a(void)
{
}

int histogram_v1_00_a::GetHistogramIndexWidth()
{
	return ceil(log2(m_NumHistogramBins));
}

vector<int> histogram_v1_00_a::GetBins()
{
	return m_Bins;
}

vector<VortexConfigurationPacket*> histogram_v1_00_a::GetConfiguration(vector<vector<int>> MinMaxConfig)
{
	vector<uint8_t> MinMaxConfigBytes;

	for (int i = 0; i < MinMaxConfig.size(); i++)
	{
		MinMaxConfigBytes.push_back((uint8_t)(MinMaxConfig[i][0] & 0xFF));
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][0] >> 8) & 0xFF));
		MinMaxConfigBytes.push_back((uint8_t)((MinMaxConfig[i][0] >> 16) & 0xFF));
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
		MinMaxConfigBytes.push_back(0);
	}

	vector<VortexConfigurationPacket*> vcp;
	vcp.push_back(new VortexConfigurationPacket(MinMaxConfigBytes.data(), MinMaxConfigBytes.size(), m_BaseAddress));

	return vcp;
}

vector<int64_t> histogram_v1_00_a::BuildHistogram(vector<vector<int64_t>> Image, int RangeMin, int RangeMax)
{
	vector<int64_t> Indices;
	m_Bins.resize(m_NumHistogramBins);

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

             m_Bins[index]++;

			 Indices.push_back(index);
         }
     }

	return Indices;
}

vector<vector<int64_t>> histogram_v1_00_a::IndexHistogram(vector<vector<int64_t>> Image, int RangeMin, int RangeMax)
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
                BackProjectedImage[y][x] = m_Bins[m_NumHistogramBins - 1];
            else if (Image[y][x] < RangeMin)
                BackProjectedImage[y][x] = m_Bins[0];
            else
                BackProjectedImage[y][x] = m_Bins[(Image[y][x] - RangeMin) >> clog2_range_width];
        }
    }

	return BackProjectedImage;
}