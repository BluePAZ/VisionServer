#ifndef __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_H__
#define __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_H__

#include <vector>
#include <array>
#include <stdint.h>
#include <string.h>
#include "multichannel_linear_interpolator_v1_00_a_interpolation_range_group.h"
#include "multichannel_linear_interpolator_v1_00_a_coefficient_table_entry.h"
#include "multichannel_linear_interpolator_v1_00_a_range_table_entry.h"
#include "VortexTypes.h"

using namespace std;

typedef double(*InterpolatedFunction)(double x);
enum ConvertMethod {CEIL, FLOOR, ROUND, NONE};

class multichannel_linear_interpolator_v1_00_a
{
public :
	multichannel_linear_interpolator_v1_00_a(uint64_t BaseAddress, vector<coefficient_table_entry*> m_CoefficientTable, vector<range_table_entry*> m_RangeTable);
	multichannel_linear_interpolator_v1_00_a(uint64_t BaseAddress, InterpolatedFunction Function, double DomainMin, double DomainMax, int DomainNumIntegerBits, int DomainNumFractionBits, int RangeNumIntegerBits, int RangeNumFractionBits, double MaxError = 0.00001);
	~multichannel_linear_interpolator_v1_00_a();

	int GetInputWidth();
	int GetOutputWidth();

	vector<vector<int64_t> > Interpolate(vector<vector<int64_t> > Image);
	int64_t InterpolateValue(int64_t Value);

	vector<VortexConfigurationPacket*> GetConfiguration();
	VortexConfigurationPacket* GetCoefficientTableConfiguration();
	VortexConfigurationPacket* GetRangeTableConfiguration();

	void WriteSimulationFiles(string Filepath, vector<vector<int64_t> > Input);
	void WriteSimulationConfigurationFiles(string Filename);
	void WriteToFile(string Filename, vector<vector<int64_t> > Data, int NumBits, int NumBitsPerLine);

	vector<coefficient_table_entry*> GetCoefficientTable();
	vector<range_table_entry*> GetRangeTable();

private:
	uint64_t m_ConfigBaseAddress[2];
	uint64_t m_BaseAddress;

	bool m_IsLocalInit;

	int m_RangeNumIntegerBits;
	int m_RangeNumFractionBits;
	bool m_RangeIsSigned;

	int m_DomainNumIntegerBits;
	int m_DomainNumFractionBits;
	bool m_DomainIsSigned;

	double m_DomainMin;
	double m_DomainMax;
	double m_DomainStep;

	double m_MaxInterpolatorError;
	int m_TableNumFractionBits;

	int64_t m_TableFractionScale;
	int64_t m_RangeFractionScale;
	int64_t m_DomainFractionScale;

	InterpolatedFunction m_IFunction;

	vector<interpolation_range_group*> m_ConsecutiveRanges;
	int m_CoefficientTableDepth;
	vector<coefficient_table_entry*> m_CoefficientTable;
	vector<range_table_entry*> m_RangeTable;

	VortexConfigurationPacket* m_CoefficientConfiguration;
	VortexConfigurationPacket* m_RangeInfoConfiguration;

	bool InitRange();
	void InitRangeTable();
	void InitCoefficientTable();

	vector<interpolation_range_group*> CompressRanges(vector<interpolation_range*> Ranges);
	int64_t ComputeMxPlusB(int64_t x, int64_t slope, int64_t b);
	double CalculateError(interpolation_range* Range);
	int64_t ToInt64(double X, int NumFractionBits, ConvertMethod ConvMethod = ConvertMethod::ROUND);
	double ToDouble(int64_t X, int NumFractionBits);
	double log2(double X);
};

#endif
