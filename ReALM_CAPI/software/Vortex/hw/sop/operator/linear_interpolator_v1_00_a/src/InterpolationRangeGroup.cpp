#include "InterpolationRangeGroup.h"

InterpolationRangeGroup::InterpolationRangeGroup()
{
}

InterpolationRangeGroup::InterpolationRangeGroup(vector<InterpolationRange*> IRanges)
{
    m_ConsecutiveRanges.insert(m_ConsecutiveRanges.end(),IRanges.begin(), IRanges.end());
}

InterpolationRangeGroup::~InterpolationRangeGroup()
{
    //dtor
}

void InterpolationRangeGroup::AddRange(vector<InterpolationRange*> IRange)
{
    m_ConsecutiveRanges.insert(m_ConsecutiveRanges.end(),IRange.begin(),IRange.end());
}

void InterpolationRangeGroup::Add(InterpolationRange* Range)
{
	m_ConsecutiveRanges.push_back(Range);
}

int InterpolationRangeGroup::GetRangeNumIntervals()
{
    return m_ConsecutiveRanges.front()->GetNumIntervals();
}

int InterpolationRangeGroup::GetNumRanges()
{
	return m_ConsecutiveRanges.size();
}

double InterpolationRangeGroup::GetRangeStart()
{
    return (m_ConsecutiveRanges.size() == 0) ? std::numeric_limits<double>::quiet_NaN() : m_ConsecutiveRanges.front()->GetRangeStart();
}

double InterpolationRangeGroup::GetRangeEnd()
{
    return (m_ConsecutiveRanges.size() == 0) ? std::numeric_limits<double>::quiet_NaN() : m_ConsecutiveRanges.back()->GetRangeEnd();
}

vector<InterpolationRange*> InterpolationRangeGroup::GetConsecutiveRanges()
{
	return m_ConsecutiveRanges;
}