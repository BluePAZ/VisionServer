/*
 *network SURFWorkConsumer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef OPENCVSURFDESCRIPTOR_WORKCONSUMER_H
#define OPENCVSURFDESCRIPTOR_WORKCONSUMER_H
//#define _GLIBCXX_USE_CXX11_ABI 0


#ifdef __WIN32
//#include "stdafx.h"
	#include <vld.h>
	#include <thread>
	#include <chrono>
#endif


#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <pthread.h>
/* for opencv rect not sure which one is needed */
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include "WorkItem/WorkItem.hpp"
#include "WorkConsumer/WorkConsumer.hpp"
#include "WorkTypeDefs.hpp"

#include "../../OpenCVSurfSupport/OpenCVSurfSupport.h"
#include "../../OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp"

#define NUM_THREADS 20

struct desc_args{
	const cv::Mat * thread_image;
	std::vector<cv::KeyPoint> * keypoints;
	cv::Mat * thread_descriptor;
};

class OpenCVSurfDescriptorWorkConsumer: public WorkConsumer {
public:
	OpenCVSurfDescriptorWorkConsumer();
	virtual ~OpenCVSurfDescriptorWorkConsumer();

	static const int surfThreshold = 13107200;

	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);
	static void * thread_descriptor(void * argss);
	cv::Mat aggregate_descriptors(struct desc_args * args);
private:
    cv::SURF * surf;
	pthread_t descriptor_threads[NUM_THREADS];
	std::vector< std::vector< cv::KeyPoint> *> * split_keypoints(std::vector<cv::KeyPoint> * keypoints);
	void configure();
	int newWidth = 1024;
	int newHeight = 768;
	int newDepth = 1;
	int currentWidth = 1024;
	int currentHeight = 768;
	int currentDepth = 1;
	//possibly not needed passsed by param
	int dataSize = 0;
};

#endif /* SURFWORKCONSUMER_SURFWORKCONSUMER_H_ */
