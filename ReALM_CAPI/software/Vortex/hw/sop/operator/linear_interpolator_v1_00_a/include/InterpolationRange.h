#ifndef INTERPOLATIONRANGE_H
#define INTERPOLATIONRANGE_H

#include <vector>
using std::vector;

class InterpolationRange
{
    public:
        InterpolationRange(double(*InterpolatedFunction)(double x), double step, double p1, double p2);
        InterpolationRange(double(*InterpolatedFunction)(double x), double step, double p1);
        virtual ~InterpolationRange();

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

#endif // INTERPOLATIONRANGE_H
