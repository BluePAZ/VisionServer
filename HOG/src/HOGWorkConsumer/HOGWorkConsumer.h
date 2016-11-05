/*
 *network SURFWorkConsumer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef HOGWORKCONSUMER_SURFWORKCONSUMER_H_
#define HOGWORKCONSUMER_SURFWORKCONSUMER_H_
#ifdef __WIN32
//#include "stdafx.h"
	#include <vld.h>
	#include <thread>
	#include <chrono>
#endif

#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <ios>
#include <fstream>
#include <stdexcept>

// for OpenCV2
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

//#include "nifsap_logger_v1_00_a.h"
#include "nif_capi_v2_00_b.h"
#include "VortexSAPTypes.h"
#include "VortexTypes.h"
#include "VortexImageDescriptor.h"
#include "hog_engine_v1_00_a.h"
#include "block_norm_v1_00_a.h"

#include "WorkItem/WorkItem.hpp"
#include "WorkConsumer/WorkConsumer.hpp"
#include "WorkTypeDefs.hpp"

#define mymax(x,y) ((x>=y)? x : y)
#define mymin(x,y) ((x>=y)? y : x)

class HOGWorkConsumer: public WorkConsumer {
public:
	HOGWorkConsumer();
	HOGWorkConsumer(IVortexNIFSAP * accel);

	virtual ~HOGWorkConsumer();

	static const int surfThreshold = 13107200;

	IVortexNIFSAP * AcceleratorInterface;
	VortexMemoryAllocation * ImageHandle;
	hog_engine_v1_00_a * RHA;

	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);

private:

	void loadDescriptorVectorFromFile(const char* fileName, cv::Mat& descriptor);
	void nonMaxSurpression(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps);
	cv::Mat convertSVM(cv::Mat& svmData);
	void generateHWImage(cv::Mat InputImg, std::string OutputFilename);
	void generateHWBins(cv::Mat Histograms, std::string OutputFilename);
	long getFileSize(FILE *file);
	void ReadImageData(std::string filename, void* dataPtr);
	void displayDotP(cv::Mat processingImage, cv::Mat dotp_output, int dotPThreshold);
	void reArrangeRHAOuput2(uint32_t* input_bins, uint32_t* output_bins, int num_histograms, int histogram_batch_size, int num_bins_per_histogram);

	void configure();
	int newWidth = 1056;
	int newHeight = 600;
	int newDepth = 4;
	int newThreshold = 13107200;
	int currentWidth = 1056;
	int currentHeight = 600;
	int currentDepth = 4;
	int currentThreshold = 13107200;
	//possibly not needed passsed by param
	int dataSize = 0;
	//double const swScale = 1.1;
	const cv::Size padding = cv::Size(0, 0);
	const cv::Size winStride = cv::Size(8, 8);
	double const hwScale = 1.1;
	enum HARDWAREHOGPARAMS : uint32_t
	{
		KERNELWIDTH = 3,
		KERNELHEIGHT = 3,
		NUMLAYERS = 15,
		BASESCALE = 1,
		SVMROIWIDTH = 14,
		SVMROICHANNELS = 31,
		CELLSIZE = 8,
		ROIPIXWIDTH = 64,
		ROIPIXHEIGHT = 128,
		DETECTIONTHRESHOLD = 122000,
		BITSPERSVMBIN = 32,
		NUMSVMBIN = 18,
		NUMRHABIN = 32,
		RHA_NCA_SCALEFACTOR = 64,
		RHABATCHSIZE = 256 // 128 histograms are stored by RHA in one batch
	};
};

#endif /* HOGWORKCONSUMER_SURFWORKCONSUMER_H_ */
