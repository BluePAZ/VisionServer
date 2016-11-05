#include "multichannel_linear_interpolator_v1_00_a_interpolation_range_group.h"

interpolation_range_group::interpolation_range_group()
{
}

interpolation_range_group::interpolation_range_group(vector<interpolation_range*> IRanges)
{
    m_ConsecutiveRanges.insert(m_ConsecutiveRanges.end(),IRanges.begin(), IRanges.end());
}

interpolation_range_group::~interpolation_range_group()
{
    //dtor
}

void interpolation_range_group::AddRange(vector<interpolation_range*> IRange)
{
    m_ConsecutiveRanges.insert(m_ConsecutiveRanges.end(),IRange.begin(),IRange.end());
}

void interpolation_range_group::Add(interpolation_range* Range)
{
	m_ConsecutiveRanges.push_back(Range);
}

int interpolation_range_group::GetRangeNumIntervals()
{
    return m_ConsecutiveRanges.front()->GetNumIntervals();
}

int interpolation_range_group::GetNumRanges()
{
	return m_ConsecutiveRanges.size();
}

double interpolation_range_group::GetRangeStart()
{
    return (m_ConsecutiveRanges.size() == 0) ? std::numeric_limits<double>::quiet_NaN() : m_ConsecutiveRanges.front()->GetRangeStart();
}

double interpolation_range_group::GetRangeEnd()
{
    return (m_ConsecutiveRanges.size() == 0) ? std::numeric_limits<double>::quiet_NaN() : m_ConsecutiveRanges.back()->GetRangeEnd();
}

vector<interpolation_range*> interpolation_range_group::GetConsecutiveRanges()
{
	return m_ConsecutiveRanges;
}
