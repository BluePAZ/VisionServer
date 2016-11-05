#include "multichannel_linear_interpolator_v1_00_a.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stack>

double multichannel_linear_interpolator_v1_00_a::ToDouble(int64_t X, int NumFractionBits)
{
	return (double)X / std::pow(2, NumFractionBits);
}

double multichannel_linear_interpolator_v1_00_a::log2(double X)
{
	return log(X) / log(2.0);
}

double round(double number)
{
    return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

multichannel_linear_interpolator_v1_00_a::multichannel_linear_interpolator_v1_00_a(uint64_t BaseAddress, vector<coefficient_table_entry*> CoefficientTable, vector<range_table_entry*> RangeTable)
{
	m_ConfigBaseAddress[0] = 0x00000;
	m_ConfigBaseAddress[1] = 0x20000;
	m_BaseAddress = BaseAddress;

	m_CoefficientTable = CoefficientTable;
	m_RangeTable = RangeTable;

	m_IsLocalInit = false;
}

multichannel_linear_interpolator_v1_00_a::multichannel_linear_interpolator_v1_00_a(uint64_t BaseAddress, InterpolatedFunction Function, double DomainMin, double DomainMax, int DomainNumIntegerBits, int DomainNumFractionBits, int RangeNumIntegerBits, int RangeNumFractionBits, double MaxError)
{
	m_ConfigBaseAddress[0] = 0x00000;
	m_ConfigBaseAddress[1] = 0x20000;
	m_TableNumFractionBits = 28;
	m_CoefficientTableDepth = 2048;
	m_IFunction = Function;
	m_BaseAddress = BaseAddress;
	m_DomainMin = DomainMin;
	m_DomainMax = DomainMax;
	m_DomainIsSigned = true;
	m_DomainNumIntegerBits = DomainNumIntegerBits;
	m_DomainNumFractionBits = DomainNumFractionBits;
	m_RangeNumIntegerBits = RangeNumIntegerBits;
	m_RangeNumFractionBits = RangeNumFractionBits;
	m_RangeIsSigned = true;
	m_TableNumFractionBits = RangeNumFractionBits;
	m_MaxInterpolatorError = MaxError;
	m_TableFractionScale = (int64_t)(pow(2,m_TableNumFractionBits));
	m_RangeFractionScale = (int64_t)(pow(2,m_RangeNumFractionBits));
	m_DomainFractionScale = (int64_t)round(pow(2,m_DomainNumFractionBits));

	InitRange();
	InitRangeTable();
	InitCoefficientTable();

	m_IsLocalInit = true;
}

multichannel_linear_interpolator_v1_00_a::~multichannel_linear_interpolator_v1_00_a()
{
	if (m_IsLocalInit)
	{
		for(std::vector<coefficient_table_entry*>::iterator it = m_CoefficientTable.begin(); it != m_CoefficientTable.end(); ++it)
			delete(*it);

		for(std::vector<range_table_entry*>::iterator it = m_RangeTable.begin(); it != m_RangeTable.end(); ++it)
			delete(*it);

		for(std::vector<interpolation_range_group*>::iterator it = m_ConsecutiveRanges.begin(); it != m_ConsecutiveRanges.end(); ++it)
		{
			vector<interpolation_range*> ranges = (*it)->GetConsecutiveRanges();
			for (std::vector<interpolation_range*>::iterator rg = ranges.begin(); rg != ranges.end(); ++rg)
			{
				delete(*rg);
			}

			delete(*it);
		}
	}
}

int multichannel_linear_interpolator_v1_00_a::GetInputWidth()
{
	return m_DomainNumIntegerBits + m_DomainNumFractionBits;
}

int multichannel_linear_interpolator_v1_00_a::GetOutputWidth()
{
	return m_RangeNumIntegerBits + m_RangeNumFractionBits;
}

vector<coefficient_table_entry*> multichannel_linear_interpolator_v1_00_a::GetCoefficientTable()
{
	return m_CoefficientTable;
}

vector<range_table_entry*> multichannel_linear_interpolator_v1_00_a::GetRangeTable()
{
	return m_RangeTable;
}

vector<vector<int64_t>> multichannel_linear_interpolator_v1_00_a::Interpolate(vector<vector<int64_t>> Image)
{
	double MaxError = 0;
	vector<vector<int64_t>> InterpolatedImage;
	InterpolatedImage.resize(Image.size());

	for (int r=0; r<Image.size(); r++)
	{
		InterpolatedImage[r].resize(Image[r].size());
		for(int c=0; c<Image[r].size(); c++)
		{
			InterpolatedImage[r][c] = InterpolateValue(Image[r][c]);
			double act_value = ToDouble(InterpolatedImage[r][c], m_RangeNumFractionBits);
			double exp_value = m_IFunction(ToDouble(Image[r][c],m_DomainNumFractionBits));

			MaxError = max(MaxError,std::abs(exp_value - act_value));
		}
	}

	return InterpolatedImage;
}

int64_t multichannel_linear_interpolator_v1_00_a::InterpolateValue(int64_t Value)
{
	range_table_entry* SelectedRange = NULL;

	for (int i=m_RangeTable.size()-1; i>=0; i--)
	{
		if (Value <= m_RangeTable[i]->GetRangeEnd())
			SelectedRange = m_RangeTable[i];
	}

	int64_t x_fxd = Value - SelectedRange->GetRangeStart();
	int CoefficientTableOffset = SelectedRange->GetRangeBaseAddress();
	int CoefficientTableIndex = (int)(x_fxd >> SelectedRange->GetRangeScaleFactor());
	int CoefficientTableAddress = CoefficientTableOffset + CoefficientTableIndex;

	coefficient_table_entry* SelectedCoefficient = m_CoefficientTable.at(CoefficientTableAddress);

	return ComputeMxPlusB(Value - SelectedCoefficient->GetXOffset(), SelectedCoefficient->GetSlope(), SelectedCoefficient->GetYOffset());
}

vector<VortexConfigurationPacket*> multichannel_linear_interpolator_v1_00_a::GetConfiguration()
{
	vector<VortexConfigurationPacket*> vcp;
	vcp.push_back(GetRangeTableConfiguration());
	vcp.push_back(GetCoefficientTableConfiguration());

	return vcp;
}

VortexConfigurationPacket* multichannel_linear_interpolator_v1_00_a::GetCoefficientTableConfiguration()
{
	VortexConfigurationPacket* vcp = new VortexConfigurationPacket(m_ConfigBaseAddress[1] + m_BaseAddress);

	for (int i=0; i<m_CoefficientTable.size(); i++)
	{
		int64_t YOffset = m_CoefficientTable[i]->GetYOffset();
		uint64_t XOffset = m_CoefficientTable[i]->GetXOffset();
		uint64_t Slope = m_CoefficientTable[i]->GetSlope();

		vcp->Add(((uint8_t*)&YOffset),6);
		vcp->Add(0);
		vcp->Add(0);
		vcp->Add(((uint8_t*)&XOffset),4);
		vcp->Add(((uint8_t*)&Slope),4);
	}

	return vcp;
}

VortexConfigurationPacket* multichannel_linear_interpolator_v1_00_a::GetRangeTableConfiguration()
{
	VortexConfigurationPacket* vcp = new VortexConfigurationPacket(m_ConfigBaseAddress[0] + m_BaseAddress);

	for (int i = 0; i<m_RangeTable.size(); i++)
	{
		uint64_t RangeEnd = m_RangeTable[i]->GetRangeEnd();
		uint64_t RangeStart = m_RangeTable[i]->GetRangeStart();
		uint64_t RangeBaseAddress = m_RangeTable[i]->GetRangeBaseAddress();
		uint64_t RangeScaleFactor = m_RangeTable[i]->GetRangeScaleFactor();

		vcp->Add(((uint8_t*)&RangeEnd),4);
		vcp->Add(((uint8_t*)&RangeStart),4);
		vcp->Add(((uint8_t*)&RangeBaseAddress),4);
		vcp->Add(((uint8_t*)&RangeScaleFactor),4);
	}

	return vcp;
}

void multichannel_linear_interpolator_v1_00_a::WriteSimulationFiles(string Filepath, vector<vector<int64_t>> Input)
{
	vector<vector<int64_t>> Output = Interpolate(Input);

	WriteToFile(Filepath + "\\input.dat", Input, 32, 128);
	WriteToFile(Filepath + "\\output.dat", Output, 64, 64);
	WriteSimulationConfigurationFiles(Filepath + "\\config.dat");
}

void multichannel_linear_interpolator_v1_00_a::WriteSimulationConfigurationFiles(string Filename)
{
	ofstream sw(Filename,std::ios_base::trunc);

	vector<VortexConfigurationPacket*> vcp = GetConfiguration();

	for (int c=0; c<vcp.size(); c++)
	{
		int ConfigStringLength;
		string* ConfigLines = vcp[c]->GetConfigString(ConfigStringLength);
		for (int i=0; i<ConfigStringLength; i++)
		{
			if ((c == vcp.size() - 1) && (i == ConfigStringLength - 1))
			{
				sw << string("1") << ConfigLines[i] << endl;
			}
			else
			{
				sw << string("0") << ConfigLines[i] << endl;
			}
		}
	}

	sw.close();
}

void multichannel_linear_interpolator_v1_00_a::WriteToFile(string Filename, vector<vector<int64_t>> Data, int NumBits, int NumBitsPerLine)
{
	ofstream sw(Filename,std::ios_base::trunc);

	if (!sw)
		return;

	stack<uint8_t> InputStack;

	for (int y=0; y<Data.size(); y++)
	{
		for (int x=0; x<Data[y].size(); x++)
		{
			uint8_t* InputBytes = (uint8_t*)&Data[y][x];
			for (int b=0; b<NumBits/8; b++)
			{
				InputStack.push(InputBytes[b]);
				if (InputStack.size() == (NumBitsPerLine/8))
				{
					while (InputStack.size() != 0)
					{
						sw << std::hex << std::setfill('0') << std::setw(2) << InputStack.top();
						InputStack.pop();
					}

					sw << endl;
				}
			}
		}
	}

	sw.close();
}

bool multichannel_linear_interpolator_v1_00_a::InitRange()
{
	int MinNumOutputFractionBits = (int)ceil(abs(log2(m_MaxInterpolatorError)));

	if (MinNumOutputFractionBits > m_RangeNumFractionBits || MinNumOutputFractionBits > m_TableNumFractionBits)
		return false;

	m_DomainStep = pow(2,-m_DomainNumFractionBits);

	vector<interpolation_range*> RangeInfo;
	RangeInfo.push_back(new interpolation_range(m_IFunction,m_DomainStep,m_DomainMin));

	interpolation_range* CurrentRange;
	double error;

	while ((CurrentRange = RangeInfo.back())->GetRangeEnd() <= m_DomainMax)
	{
		if ((error = CalculateError(CurrentRange)) > m_MaxInterpolatorError)
		{
			if (CurrentRange->GetNumInterpolatedPoints() == 1)
				return false;

			RangeInfo.push_back(new interpolation_range(m_IFunction, m_DomainStep, CurrentRange->RemoveHalf()));
		}
		else
		{
			CurrentRange->AddDouble();
		}
	}

	//delete m_ConsecutiveRanges;
	m_ConsecutiveRanges = CompressRanges(RangeInfo);

	return true;

}

void multichannel_linear_interpolator_v1_00_a::InitRangeTable()
{
	m_RangeTable.clear();

	int RangeTableOffset = 0;

	for (int i=0; i<m_ConsecutiveRanges.size(); i++)
	{
		int64_t RangePoints[2] =
		{
			ToInt64(m_ConsecutiveRanges[i]->GetRangeStart(), m_DomainNumFractionBits),
			ToInt64(m_ConsecutiveRanges[i]->GetRangeEnd(), m_DomainNumFractionBits)
		};

		int RangeShift = (int)log2(m_ConsecutiveRanges[i]->GetRangeNumIntervals())+1;

		m_RangeTable.push_back(new range_table_entry(RangePoints[0],RangePoints[1],RangeShift,RangeTableOffset));
		RangeTableOffset += m_ConsecutiveRanges[i]->GetNumRanges();
	}

	sort(m_RangeTable.begin(),m_RangeTable.end(),range_table_entry::Compare);
}

void multichannel_linear_interpolator_v1_00_a::InitCoefficientTable()
{
	int CoefficientTableIndex = 0;
	int CoefficientTableSize = 0;
	m_CoefficientTable.clear();

	for(std::vector<interpolation_range_group*>::iterator j=m_ConsecutiveRanges.begin();j!=m_ConsecutiveRanges.end();++j)
		CoefficientTableSize += (*j)->GetNumRanges();

	for (int g=0; g<m_ConsecutiveRanges.size(); g++)
	{
		for (int r=0; r<m_ConsecutiveRanges[g]->GetNumRanges(); r++)
		{
			interpolation_range* ir = m_ConsecutiveRanges[g]->GetConsecutiveRanges()[r];
			int64_t Coefficients[3] =
			{
				ToInt64(ir->GetRangeStart(),m_DomainNumFractionBits),
				ToInt64(ir->GetIntercept(), m_TableNumFractionBits),
				ToInt64(ir->GetSlope(), m_TableNumFractionBits)
			};

			m_CoefficientTable.push_back(new coefficient_table_entry(Coefficients[0],Coefficients[1],Coefficients[2],ir));
		}
	}
}

vector<interpolation_range_group*> multichannel_linear_interpolator_v1_00_a::CompressRanges(vector<interpolation_range*> Ranges)
{
	int index = 0;
	vector<interpolation_range_group*> ConsecutiveRanges;

	while (index < Ranges.size())
	{
		int collapsed_ranges_count = 0;
		interpolation_range_group* irg = NULL;
		for(std::vector<interpolation_range*>::iterator it = Ranges.begin() + index; it != Ranges.end(); ++it)
		{
			if ((*it)->GetNumIntervals() == Ranges[index]->GetNumIntervals())
			{
				collapsed_ranges_count++;
				if (irg == NULL)
					irg = new interpolation_range_group();

				irg->Add(*it);
			}
			else
			{
				break;
			}
		}
		index += collapsed_ranges_count;
		ConsecutiveRanges.push_back(irg);
	}
	return ConsecutiveRanges;
}

int64_t multichannel_linear_interpolator_v1_00_a::ComputeMxPlusB(int64_t x, int64_t slope, int64_t b)
{
	int64_t mx = slope * x;
	int64_t mx_plus_b = mx + b;

	return ((mx_plus_b / (double)m_TableFractionScale) * m_RangeFractionScale);
}

double multichannel_linear_interpolator_v1_00_a::CalculateError(interpolation_range* ir)
{
	double MaxError = 0;

	if (ir->GetNumInterpolatedPoints() == 1)
		return abs(m_IFunction(ir->GetRangeStart()) - ((int64_t)round(m_IFunction(ir->GetRangeStart())* m_RangeFractionScale) / (double)m_RangeFractionScale));

	int64_t fxd_intercept = (int64_t)round(ir->GetIntercept() * m_TableFractionScale);
	int64_t startPoint = (int64_t)round(ir->m_Points[0] * m_DomainFractionScale);

	for (int i=0; i<ir->m_Points.size(); i++)
	{
		int64_t fxd_slope = (int64_t)round(ir->GetSlope() * m_TableFractionScale);
		//int64_t fxd_slope = ToInt64(ir->GetSlope(), m_TableNumFractionBits,ConvertMethod::ROUND);

		int64_t fxd_x = (ir->m_Points[i]*m_DomainFractionScale) - startPoint;
		//int64_t fxd_x = ToInt64(xPoints[i], m_DomainNumFractionBits, ConvertMethod::NONE) - ToInt64(ir->GetRangeStart(), m_DomainNumFractionBits, ConvertMethod::ROUND);

		double dbl_y = (ComputeMxPlusB(fxd_x,fxd_slope,fxd_intercept) / (double)m_RangeFractionScale);
		//double dbl_y = ToDouble(ComputeMxPlusB(fxd_x,fxd_slope,fxd_intercept), m_RangeNumFractionBits);

		MaxError = max(MaxError, abs(m_IFunction(ir->m_Points[i]) - dbl_y));
	}

	return MaxError;
}

int64_t multichannel_linear_interpolator_v1_00_a::ToInt64(double X, int NumFractionBits, ConvertMethod ConvMethod)
{
	if (ConvMethod == ConvertMethod::ROUND)
		return (int64_t)round(pow(2,NumFractionBits) * X);
	else if (ConvMethod == ConvertMethod::FLOOR)
		return (int64_t)floor(pow(2,NumFractionBits) * X);
	else if (ConvMethod == ConvertMethod::CEIL)
		return (int64_t)ceil(pow(2,NumFractionBits) * X);
	else
		return (int64_t)(pow(2,NumFractionBits) * X);
}

