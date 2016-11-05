#include "AIMGaborKernel.h"
#define _USE_MATH_DEFINES //Needed for M_PI in VS
#include <math.h>

/// <summary>
/// Creat a Gabor filter bank consisting of m_NumScales * m_NumOrientations kernels.
/// </summary>
vector<vector<vector<vector<int16_t>>>> AIMGaborKernel::CreateGaborFilterBank(vector<int> Scales, int NumOrientations)
{
	if ((NumOrientations == 0) || (Scales.size() == 0))
        return vector<vector<vector<vector<int16_t>>>>();
            
    vector<vector<vector<vector<int16_t>>>> Kernels(Scales.size());

    for (int s = 0; s < Scales.size(); s++)
    {
        Kernels[s].resize(NumOrientations);
        for (int o = 0; o < NumOrientations; o++)
        {
            double sigma = (0.0036 * pow(Scales[s], 2.0)) + 0.35 * Scales[s] + 0.18;
            double wavelength = sigma / 0.8;
            double angle = ((double)o) * (M_PI / (double)NumOrientations);
            Kernels[s][o] = CreateGaborFilter(angle, true, wavelength, sigma, Scales[s], 3);
        }
    }

    return Kernels;
}

/// <summary>
/// Create a single Gabor filter
/// </summary>
/// <param name="Angle">Orientation angle in PI radians</param>
/// <param name="Odd">Polarity of the filter</param>
/// <param name="Wavelength">Wavelength</param>
/// <param name="Sigma">Sigma</param>
/// <param name="KernelSize">KernelSize</param>
/// <param name="Gamma">Gamma</param>
/// <returns></returns>
vector<vector<int16_t>> AIMGaborKernel::CreateGaborFilter(double Angle, bool Odd, double Wavelength, double Sigma, int KernelSize, double Gamma)
{
	int center = (KernelSize - 1) / 2;
	vector<vector<double>> u1(KernelSize); 
	vector<vector<double>> u2(KernelSize); 
	vector<vector<double>> gaussian(KernelSize); 
	vector<vector<double>> wave(KernelSize); 
	vector<vector<double>> gabor(KernelSize); 
	vector<vector<int16_t>> gaborFxd(KernelSize);

	for (int i=0; i<KernelSize; i++)
	{
		u1[i].resize(KernelSize);
		u2[i].resize(KernelSize);
		gaussian[i].resize(KernelSize);
		wave[i].resize(KernelSize);
		gabor[i].resize(KernelSize);
		gaborFxd[i].resize(KernelSize);
	}

	double normX = cos(Angle);
    double normY = sin(Angle);

    double gamma_sqr = Gamma * Gamma;
    double sigma_sqr = Sigma * Sigma;
    double half_kernelsize_sqr = (((double)KernelSize) / 2) * (((double)KernelSize) / 2);

    double energy = 0;
	
	for (int row = (0 - center); row < KernelSize - center; row++)
    {
        for (int col = (0 - center); col < KernelSize - center; col++)
        {
            double u1e = u1[row + center][col + center] = row * normX + col * normY;
            double u2e = u2[row + center][col + center] = -row * normY + row * normX;
            gaussian[row + center][col + center] = exp(-(u1e * u1e + gamma_sqr * (u2e * u2e)) / (2 * sigma_sqr));
            if (Odd)
                wave[row + center][col + center] = cos(2 * M_PI / Wavelength * u1e - M_PI / 2);
            else
                wave[row + center][col + center] = cos(2 * M_PI / Wavelength * u1e);

            gabor[row + center][col + center] = gaussian[row + center][col + center] * wave[row + center][col + center];
            double mag = ((double)row * (double)row) + ((double)col * (double)col);
            gabor[row + center][col + center] = (mag > half_kernelsize_sqr) ? 0 : gabor[row + center][col + center];
            energy += pow(gabor[row + center][col + center], 2.0);
        }
    }

	energy = sqrt(energy);
    

    for (int row = 0; row < gabor.size(); row++)
    {
        for (int col = 0; col < gabor[0].size(); col++)
        {
            int16_t Number = gaborFxd[row][col] = (int16_t)((gabor[row][col] /= energy) * pow(2, m_KernelNumFractionalBits));
        }
    }

    return gaborFxd;
}
