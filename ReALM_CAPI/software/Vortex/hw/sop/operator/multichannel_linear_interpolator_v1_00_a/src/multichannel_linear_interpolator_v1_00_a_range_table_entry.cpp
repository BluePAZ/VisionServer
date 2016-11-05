#include "multichannel_linear_interpolator_v1_00_a_range_table_entry.h"

bool range_table_entry::Compare(range_table_entry* item1, range_table_entry* item2)
{
	return (item1->GetRangeEnd() < item2->GetRangeEnd());
}

range_table_entry::range_table_entry(int64_t RangeStart, int64_t RangeEnd, int RangeScaleFactor, int RangeBaseAddress)
{
    m_RangeStart = RangeStart;
    m_RangeEnd = RangeEnd;
    m_RangeScaleFactor = RangeScaleFactor;
    m_RangeBaseAddress = RangeBaseAddress;
}

range_table_entry::~range_table_entry()
{
    //dtor
}

int64_t range_table_entry::GetRangeStart()
{
    return m_RangeStart;
}

int64_t range_table_entry::GetRangeEnd()
{
    return m_RangeEnd;
}

int range_table_entry::GetRangeBaseAddress()
{
    return m_RangeBaseAddress;
}

int range_table_entry::GetRangeScaleFactor()
{
    return m_RangeScaleFactor;
}

void range_table_entry::SetRangeStart(int64_t RangeStart)
{
    m_RangeStart = RangeStart;
}

void range_table_entry::SetRangeEnd(int64_t RangeEnd)
{
    m_RangeEnd = RangeEnd;
}

void range_table_entry::SetRangeBaseAddress(int RangeBaseAddress)
{
    m_RangeBaseAddress = RangeBaseAddress;
}

void range_table_entry::SetRangeScaleFactor(int RangeScaleFactor)
{
    m_RangeScaleFactor = RangeScaleFactor;
}
