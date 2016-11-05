#include "multichannel_linear_interpolator_v1_00_a_coefficient_table_entry.h"


coefficient_table_entry::coefficient_table_entry(int64_t XOffset, int64_t YOffset, int64_t Slope, interpolation_range* RangeInfo)
{
    m_XOffset = XOffset;
    m_YOffset = YOffset;
    m_Slope = Slope;
    m_RangeInfo = RangeInfo;
}

coefficient_table_entry::~coefficient_table_entry()
{
    //dtor
}

int64_t coefficient_table_entry::GetXOffset()
{
    return m_XOffset;
}

int64_t coefficient_table_entry::GetYOffset()
{
    return m_YOffset;
}

int64_t coefficient_table_entry::GetSlope()
{
    return m_Slope;
}

interpolation_range* coefficient_table_entry::GetRangeInfo()
{
    return m_RangeInfo;
}

void coefficient_table_entry::SetXOffset(int64_t XOffset)
{
    m_XOffset = XOffset;
}

void coefficient_table_entry::SetYOffset(int64_t YOffset)
{
    m_YOffset = YOffset;
}

void coefficient_table_entry::SetSlope(int64_t Slope)
{
    m_Slope = Slope;
}

void coefficient_table_entry::SetRangeInfo(interpolation_range* RangeInfo)
{
    m_RangeInfo = RangeInfo;
}
