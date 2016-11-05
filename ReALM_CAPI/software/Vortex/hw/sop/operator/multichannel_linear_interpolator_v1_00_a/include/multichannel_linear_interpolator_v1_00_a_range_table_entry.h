#ifndef __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_RANGE_TABLE_ENTRY_H
#define __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_RANGE_TABLE_ENTRY_H

#include <stdint.h>

class range_table_entry
{
    public:
        range_table_entry(int64_t RangeStart, int64_t RangeEnd, int RangeScaleFactor, int RangeBaseAddress);
        virtual ~range_table_entry();

		static bool Compare(range_table_entry* item1, range_table_entry* item2);

        int64_t GetRangeStart();
        int64_t GetRangeEnd();
        int GetRangeBaseAddress();
        int GetRangeScaleFactor();

        void SetRangeStart(int64_t RangeStart);
        void SetRangeEnd(int64_t RangeEnd);
        void SetRangeBaseAddress(int RangeBaseAddress);
        void SetRangeScaleFactor(int RangeScaleFactor);
    protected:
    private:
        int64_t m_RangeStart;
        int64_t m_RangeEnd;
        int m_RangeScaleFactor;
        int m_RangeBaseAddress;
};

#endif // __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_RANGE_TABLE_ENTRY_H
