#ifndef COEFFICIENTTABLEENTRY_H
#define COEFFICIENTTABLEENTRY_H

#include "InterpolationRange.h"
#include <stdint.h>

class CoefficientTableEntry
{
    public:
        CoefficientTableEntry(int64_t XOffset, int64_t YOffset, int64_t Slope, InterpolationRange* RangeInfo);
        virtual ~CoefficientTableEntry();
        int64_t GetXOffset();
        int64_t GetYOffset();
        int64_t GetSlope();
        InterpolationRange* GetRangeInfo();
        void SetXOffset(int64_t XOffset);
        void SetYOffset(int64_t YOffset);
        void SetSlope(int64_t Slope);
        void SetRangeInfo(InterpolationRange* RangeInfo);

    protected:
    private:
        int64_t m_XOffset;
        int64_t m_YOffset;
        int64_t m_Slope;
        InterpolationRange* m_RangeInfo;
};

#endif // COEFFICIENTTABLEENTRY_H
