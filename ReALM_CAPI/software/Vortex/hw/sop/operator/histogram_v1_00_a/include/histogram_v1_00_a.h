#ifndef __HISTOGRAM_V1_00_A_H__
#define __HISTOGRAM_V1_00_A_H__

#include <vector>
#include <stdint.h>
#include <math.h>
#include "VortexTypes.h"

using namespace std;
class histogram_v1_00_a
{
public:
	histogram_v1_00_a(void);
	histogram_v1_00_a(uint64_t BaseAddress, int NumHistogramBins);
	~histogram_v1_00_a(void);

	int GetHistogramIndexWidth();
	vector<int> GetBins();
	vector<VortexConfigurationPacket*> GetConfiguration(vector<vector<int>> MinMaxConfig);
	vector<int64_t> BuildHistogram(vector<vector<int64_t>> Image, int RangeMin, int RangeMax);
	vector<vector<int64_t>> IndexHistogram(vector<vector<int64_t>> Image, int RangeMin, int RangeMax);
private:
	int m_NumHistogramBins;
	uint64_t m_BaseAddress;
	vector<int> m_Bins;

	double log2(double X);
	
};

#endif
