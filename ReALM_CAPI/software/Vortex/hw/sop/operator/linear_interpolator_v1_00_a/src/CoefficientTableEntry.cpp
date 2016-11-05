#include "CoefficientTableEntry.h"


CoefficientTableEntry::CoefficientTableEntry(int64_t XOffset, int64_t YOffset, int64_t Slope, InterpolationRange* RangeInfo)
{
    m_XOffset = XOffset;
    m_YOffset = YOffset;
    m_Slope = Slope;
    m_RangeInfo = RangeInfo;
}

CoefficientTableEntry::~CoefficientTableEntry()
{
    //dtor
}

int64_t CoefficientTableEntry::GetXOffset()
{
    return m_XOffset;
}

int64_t CoefficientTableEntry::GetYOffset()
{
    return m_YOffset;
}

int64_t CoefficientTableEntry::GetSlope()
{
    return m_Slope;
}

InterpolationRange* CoefficientTableEntry::GetRangeInfo()
{
    return m_RangeInfo;
}

void CoefficientTableEntry::SetXOffset(int64_t XOffset)
{
    m_XOffset = XOffset;
}

void CoefficientTableEntry::SetYOffset(int64_t YOffset)
{
    m_YOffset = YOffset;
}

void CoefficientTableEntry::SetSlope(int64_t Slope)
{
    m_Slope = Slope;
}

void CoefficientTableEntry::SetRangeInfo(InterpolationRange* RangeInfo)
{
    m_RangeInfo = RangeInfo;
}
