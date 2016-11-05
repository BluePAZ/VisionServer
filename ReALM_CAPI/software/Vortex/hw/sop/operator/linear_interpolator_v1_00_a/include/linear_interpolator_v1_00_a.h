#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H

#include <vector>
#include <array>
#include <stdint.h>
#include <string.h>
#include "InterpolationRangeGroup.h"
#include "CoefficientTableEntry.h"
#include "RangeTableEntry.h"
#include "VortexTypes.h"

using namespace std;

typedef double(*InterpolatedFunction)(double x);
typedef enum ConvertMethod {CEIL, FLOOR, ROUND, NONE};

class linear_interpolator_v1_00_a
{
public :
	linear_interpolator_v1_00_a(uint64_t BaseAddress, vector<CoefficientTableEntry*> m_CoefficientTable, vector<RangeTableEntry*> m_RangeTable);
	linear_interpolator_v1_00_a(uint64_t BaseAddress, InterpolatedFunction Function, double DomainMin, double DomainMax, int DomainNumIntegerBits, int DomainNumFractionBits, int RangeNumIntegerBits, int RangeNumFractionBits, double MaxError = 0.00001);
	~linear_interpolator_v1_00_a();

	int GetInputWidth();
	int GetOutputWidth();

	vector<vector<int64_t>> Interpolate(vector<vector<int64_t>> Image);
	int64_t InterpolateValue(int64_t Value);

	vector<VortexConfigurationPacket*> GetConfiguration();
	VortexConfigurationPacket* GetCoefficientTableConfiguration();
	VortexConfigurationPacket* GetRangeTableConfiguration();

	void WriteSimulationFiles(string Filepath, vector<vector<int64_t>> Input);
	void WriteSimulationConfigurationFiles(string Filename);
	void WriteToFile(string Filename, vector<vector<int64_t>> Data, int NumBits, int NumBitsPerLine);
	
	vector<CoefficientTableEntry*> GetCoefficientTable();
	vector<RangeTableEntry*> GetRangeTable();

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

	vector<InterpolationRangeGroup*> m_ConsecutiveRanges;
	int m_CoefficientTableDepth;
	vector<CoefficientTableEntry*> m_CoefficientTable;
	vector<RangeTableEntry*> m_RangeTable;

	VortexConfigurationPacket* m_CoefficientConfiguration;
	VortexConfigurationPacket* m_RangeInfoConfiguration;

	bool InitRange();
	void InitRangeTable();
	void InitCoefficientTable();

	vector<InterpolationRangeGroup*> CompressRanges(vector<InterpolationRange*> Ranges);
	int64_t ComputeMxPlusB(int64_t x, int64_t slope, int64_t b);
	double CalculateError(InterpolationRange* Range);
	int64_t ToInt64(double X, int NumFractionBits, ConvertMethod ConvMethod = ConvertMethod::ROUND);
	double ToDouble(int64_t X, int NumFractionBits);
	double log2(double X);
};

#endif