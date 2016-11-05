#ifndef __MULTICHANNEL_HISTOGRAM_V1_00_A_H__
#define __MULTICHANNEL_HISTOGRAM_V1_00_A_H__

#include <vector>
#include <stdint.h>
#include "VortexTypes.h"
#include "multichannel_histogram_v1_00_a_histogram_partition.h"

using namespace std;

class multichannel_histogram_v1_00_a
{
public:
	multichannel_histogram_v1_00_a(uint64_t BaseAddress, int NumHistograms, int NumHistogramBins);
	~multichannel_histogram_v1_00_a(void);

	int GetHistogramIndexWidth();
	int GetNumHistogramBins();
	void SetPartition(histogram_partition* Partitions);
	void Init();
	vector<VortexConfigurationPacket*> GetConfiguration(vector<vector<int> > MinMaxConfig, int ImageWidth, int ImageHeight, histogram_partition* Partitions);
	vector<int64_t> BuildHistogram(vector<vector<int64_t> > Image, int RangeMin, int RangeMax);
	vector<vector<int64_t> > IndexHistogram(vector<vector<int64_t> > Image, int RangeMin, int RangeMax);
	vector<vector<int64_t> > GenerateHistogramTestData(vector<vector<int64_t> > Image, vector<int> MinMax, string OutputFilepath);

private:
	int m_NumHistograms;
	int m_NumHistogramBins;
	uint64_t m_BaseAddress;
	uint64_t m_MinMaxConfigBaseAddress;
	uint64_t m_PartitionConfigBaseAddress;
	uint64_t m_ImageSizeConfigBaseAddress;
	vector<vector<int> > m_Bins;

	histogram_partition* m_Partitions;
	vector<uint8_t> GetImageSizeConfiguration(int ImageWidth, int ImageHeight);
	vector<uint8_t> GetMinMaxConfiguration(vector<vector<int> > MinMaxConfig);
	vector<uint8_t> GetPartitionConfiguration();
	void WriteToFile(vector<vector<int64_t> > Image, int OutputDatawidth, ofstream* OutputFile);
	void WriteToFile(vector<int64_t> Image, int OutputDatawidth, ofstream* OutputFile);

	double log2(double X);

};

#endif
