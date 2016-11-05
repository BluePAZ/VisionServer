#include "RangeTableEntry.h"

bool RangeTableEntry::Compare(RangeTableEntry* item1, RangeTableEntry* item2)
{
	return (item1->GetRangeEnd() < item2->GetRangeEnd());
}

RangeTableEntry::RangeTableEntry(int64_t RangeStart, int64_t RangeEnd, int RangeScaleFactor, int RangeBaseAddress)
{
    m_RangeStart = RangeStart;
    m_RangeEnd = RangeEnd;
    m_RangeScaleFactor = RangeScaleFactor;
    m_RangeBaseAddress = RangeBaseAddress;
}

RangeTableEntry::~RangeTableEntry()
{
    //dtor
}

int64_t RangeTableEntry::GetRangeStart()
{
    return m_RangeStart;
}

int64_t RangeTableEntry::GetRangeEnd()
{
    return m_RangeEnd;
}

int RangeTableEntry::GetRangeBaseAddress()
{
    return m_RangeBaseAddress;
}

int RangeTableEntry::GetRangeScaleFactor()
{
    return m_RangeScaleFactor;
}

void RangeTableEntry::SetRangeStart(int64_t RangeStart)
{
    m_RangeStart = RangeStart;
}

void RangeTableEntry::SetRangeEnd(int64_t RangeEnd)
{
    m_RangeEnd = RangeEnd;
}

void RangeTableEntry::SetRangeBaseAddress(int RangeBaseAddress)
{
    m_RangeBaseAddress = RangeBaseAddress;
}

void RangeTableEntry::SetRangeScaleFactor(int RangeScaleFactor)
{
    m_RangeScaleFactor = RangeScaleFactor;
}
