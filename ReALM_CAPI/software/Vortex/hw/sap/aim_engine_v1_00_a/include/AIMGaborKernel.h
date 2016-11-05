#ifndef __AIM_GABOR_KERNEL_H__
#define __AIM_GABOR_KERNEL_H__

#include <stdint.h>
#include <vector>

using namespace std;

class AIMGaborKernel
{
public:
	static vector<vector<vector<vector<int16_t>>>> CreateGaborFilterBank(vector<int> Scales, int NumOrientations);
	static vector<vector<int16_t>> CreateGaborFilter(double Angle, bool Odd, double Wavelength, double Sigma, int KernelSize, double Gamma);
private:
	static const int m_KernelNumFractionalBits = 14;
};

#endif
