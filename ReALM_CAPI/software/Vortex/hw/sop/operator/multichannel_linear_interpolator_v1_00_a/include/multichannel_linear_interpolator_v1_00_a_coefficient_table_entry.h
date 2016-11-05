#ifndef __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_COEFFICIENT_TABLE_ENTRY_H
#define __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_COEFFICIENT_TABLE_ENTRY_H

#include "multichannel_linear_interpolator_v1_00_a_interpolation_range.h"
#include <stdint.h>

class coefficient_table_entry
{
    public:
        coefficient_table_entry(int64_t XOffset, int64_t YOffset, int64_t Slope, interpolation_range* RangeInfo);
        virtual ~coefficient_table_entry();
        int64_t GetXOffset();
        int64_t GetYOffset();
        int64_t GetSlope();
        interpolation_range* GetRangeInfo();
        void SetXOffset(int64_t XOffset);
        void SetYOffset(int64_t YOffset);
        void SetSlope(int64_t Slope);
        void SetRangeInfo(interpolation_range* RangeInfo);

    protected:
    private:
        int64_t m_XOffset;
        int64_t m_YOffset;
        int64_t m_Slope;
        interpolation_range* m_RangeInfo;
};

#endif // __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_COEFFICIENT_TABLE_ENTRY_H
