#include "multichannel_linear_interpolator_v1_00_a_interpolation_range.h"

interpolation_range::interpolation_range(double(*InterpolatedFunction)(double x), double step, double p1, double p2)
{
    ptrInterpolatedFunction = InterpolatedFunction;
    SetRangeStep(step);
    AddEnd(p1);
    AddEnd(p2);
}

interpolation_range::interpolation_range(double(*InterpolatedFunction)(double x), double step, double p1)
{
    ptrInterpolatedFunction = InterpolatedFunction;
    SetRangeStep(step);
    AddEnd(p1);
}

interpolation_range::~interpolation_range()
{
    //dtor
}

vector<double> interpolation_range::GetPoints()
{
    return m_Points;
}

int interpolation_range::GetNumInterpolatedPoints()
{
    return m_Points.size();
}

int interpolation_range::GetNumIntervals()
{
    return GetNumInterpolatedPoints() - 1;
}

double interpolation_range::GetRangeStart()
{
    return m_Points.at(0);//.front();
}

double interpolation_range::GetRangeEnd()
{
    return m_Points.back();
}

double interpolation_range::GetRangeStep()
{
    return m_RangeStep;
}

double interpolation_range::GetSlope()
{
    return m_Slope;
}

double interpolation_range::GetIntercept()
{
	return m_Intercept;
}

void interpolation_range::SetSlope(double slope)
{
    m_Slope = slope;
}

void interpolation_range::SetIntercept(double intercept)
{
    m_Intercept = intercept;
}

void interpolation_range::SetRangeStep(double step)
{
    m_RangeStep = step;
}

void interpolation_range::AddDouble()
{
    unsigned int CurrentCount = m_Points.size();
    unsigned int TargetCount = (CurrentCount == 0) ? 1 : CurrentCount*2;

	double newPoint = m_Points.back();

	int currentSize = m_Points.size();

	if (currentSize < TargetCount)
	{
		m_Points.resize(TargetCount);

		for(int i=currentSize; i<TargetCount; i++)
		{
			newPoint += GetRangeStep();
			m_Points[i]=newPoint;
		}
	}

	/*
    while (m_Points.size() < TargetCount)
	{

        m_Points.push_back(m_Points.back() +  GetRangeStep());
	}
	*/
    UpdateSlope();
}

void interpolation_range::AddEnd(double point)
{
    m_Points.push_back(point);
    UpdateSlope();
}

double interpolation_range::RemoveHalf()
{
    int split_point_index = m_Points.size() / 2;
    double split_point = m_Points[split_point_index];

    m_Points.erase(m_Points.begin() + split_point_index, m_Points.begin() + m_Points.size());

    UpdateSlope();

    return split_point;
}

void interpolation_range::UpdateSlope()
{
	double RangeEnd = GetRangeEnd();
	double RangeStart = GetRangeStart();

    double y0 = ptrInterpolatedFunction(RangeStart);
    double y1 = ptrInterpolatedFunction(RangeEnd);

    m_Slope = (y1-y0) / (RangeEnd-RangeStart);
    m_Intercept = y0;
}
