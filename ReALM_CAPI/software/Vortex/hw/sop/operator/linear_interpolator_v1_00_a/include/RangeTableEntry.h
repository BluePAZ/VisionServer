#ifndef RANGETABLEENTRY_H
#define RANGETABLEENTRY_H

#include <stdint.h>

class RangeTableEntry
{
    public:
        RangeTableEntry(int64_t RangeStart, int64_t RangeEnd, int RangeScaleFactor, int RangeBaseAddress);
        virtual ~RangeTableEntry();

		static bool Compare(RangeTableEntry* item1, RangeTableEntry* item2);

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

#endif // RANGETABLEENTRY_H
