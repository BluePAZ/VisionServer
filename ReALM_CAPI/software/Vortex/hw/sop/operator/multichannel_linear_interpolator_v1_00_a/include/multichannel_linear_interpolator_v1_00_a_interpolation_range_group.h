#ifndef __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_INTERPOLATION_RANGE_GROUP_H
#define __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_INTERPOLATION_RANGE_GROUP_H

#include <vector>
#include <limits>
#include "multichannel_linear_interpolator_v1_00_a_interpolation_range.h"

using std::vector;

class interpolation_range_group
{
    public:
        interpolation_range_group();
        interpolation_range_group(vector<interpolation_range*> IRanges);
        virtual ~interpolation_range_group();

        int GetRangeNumIntervals();
        int GetNumRanges();
		vector<interpolation_range*> GetConsecutiveRanges();
        double GetRangeStart();
        double GetRangeEnd();
        void AddRange(vector<interpolation_range*> IRange);
		void Add(interpolation_range* IRange);

    protected:
    private:
		vector<interpolation_range*> m_ConsecutiveRanges;
};

#endif // __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_INTERPOLATION_RANGE_GROUP_H
