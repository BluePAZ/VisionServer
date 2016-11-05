#include "InterpolationRange.h"

InterpolationRange::InterpolationRange(double(*InterpolatedFunction)(double x), double step, double p1, double p2)
{
    ptrInterpolatedFunction = InterpolatedFunction;
    SetRangeStep(step);
    AddEnd(p1);
    AddEnd(p2);
}

InterpolationRange::InterpolationRange(double(*InterpolatedFunction)(double x), double step, double p1)
{
    ptrInterpolatedFunction = InterpolatedFunction;
    SetRangeStep(step);
    AddEnd(p1);
}

InterpolationRange::~InterpolationRange()
{
    //dtor
}

vector<double> InterpolationRange::GetPoints()
{
    return m_Points;
}

int InterpolationRange::GetNumInterpolatedPoints()
{
    return m_Points.size();
}

int InterpolationRange::GetNumIntervals()
{
    return GetNumInterpolatedPoints() - 1;
}

double InterpolationRange::GetRangeStart()
{
    return m_Points.at(0);//.front();
}

double InterpolationRange::GetRangeEnd()
{
    return m_Points.back();
}

double InterpolationRange::GetRangeStep()
{
    return m_RangeStep;
}

double InterpolationRange::GetSlope()
{
    return m_Slope;
}

double InterpolationRange::GetIntercept()
{
	return m_Intercept;
}

void InterpolationRange::SetSlope(double slope)
{
    m_Slope = slope;
}

void InterpolationRange::SetIntercept(double intercept)
{
    m_Intercept = intercept;
}

void InterpolationRange::SetRangeStep(double step)
{
    m_RangeStep = step;
}

void InterpolationRange::AddDouble()
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

void InterpolationRange::AddEnd(double point)
{
    m_Points.push_back(point);
    UpdateSlope();
}

double InterpolationRange::RemoveHalf()
{
    int split_point_index = m_Points.size() / 2;
    double split_point = m_Points[split_point_index];

    m_Points.erase(m_Points.begin() + split_point_index, m_Points.begin() + m_Points.size());

    UpdateSlope();

    return split_point;
}

void InterpolationRange::UpdateSlope()
{
	double RangeEnd = GetRangeEnd();
	double RangeStart = GetRangeStart();

    double y0 = ptrInterpolatedFunction(RangeStart);
    double y1 = ptrInterpolatedFunction(RangeEnd);

    m_Slope = (y1-y0) / (RangeEnd-RangeStart);
    m_Intercept = y0;
}
