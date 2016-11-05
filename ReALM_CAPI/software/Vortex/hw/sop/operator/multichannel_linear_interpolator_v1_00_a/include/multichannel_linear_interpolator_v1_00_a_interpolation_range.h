#ifndef __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_INTERPOLATION_RANGE_H
#define __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_INTERPOLATION_RANGE_H

#include <vector>
using std::vector;

class interpolation_range
{
    public:
        interpolation_range(double(*InterpolatedFunction)(double x), double step, double p1, double p2);
        interpolation_range(double(*InterpolatedFunction)(double x), double step, double p1);
        virtual ~interpolation_range();

        double GetRangeStart();
        double GetRangeEnd();
        double GetRangeStep();
        double GetSlope();
		double GetIntercept();
        int GetNumInterpolatedPoints();
        int GetNumIntervals();
        vector<double> GetPoints();
        void SetRangeStep(double step);
        void AddDouble();
        void AddEnd(double point);
        double RemoveHalf();
		vector<double> m_Points;
    protected:
    private:
        double m_RangeStep;
        double m_Slope;
        double m_Intercept;
        double (*ptrInterpolatedFunction)(double x);
        void SetSlope(double slope);
        void SetIntercept(double intercept);
        void UpdateSlope();
};

#endif // __MULTICHANNEL_LINEAR_INTERPOLATOR_V1_00_A_INTERPOLATION_RANGE_H
