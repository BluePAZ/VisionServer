#ifndef INTERPOLATIONRANGEGROUP_H
#define INTERPOLATIONRANGEGROUP_H

#include <vector>
#include <limits>
#include "InterpolationRange.h"

using std::vector;

class InterpolationRangeGroup
{
    public:
        InterpolationRangeGroup();
        InterpolationRangeGroup(vector<InterpolationRange*> IRanges);
        virtual ~InterpolationRangeGroup();

        int GetRangeNumIntervals();
        int GetNumRanges();
		vector<InterpolationRange*> GetConsecutiveRanges();
        double GetRangeStart();
        double GetRangeEnd();
        void AddRange(vector<InterpolationRange*> IRange);
		void Add(InterpolationRange* IRange);

    protected:
    private:
		vector<InterpolationRange*> m_ConsecutiveRanges;
};

#endif // INTERPOLATIONRANGEGROUP_H
