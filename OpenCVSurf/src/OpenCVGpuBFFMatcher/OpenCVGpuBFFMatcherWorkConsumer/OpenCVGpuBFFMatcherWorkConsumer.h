/*
 *network SURFWorkConsumer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef __OPENCVGPUBFFMATCHER_WORKCONSUMER_H__
#define __OPENCVGPUBFFMATCHER_WORKCONSUMER_H__

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

#include "opencv2/gpu/gpu.hpp"
#include "opencv2/nonfree/gpu.hpp"

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
#include "PointF.h"


#include "../../OpenCVSurfFullDemoWorkItem/OpenCVSurfFullDemoWorkItem.hpp"


#include "../../OpenCVSurfSupport/OpenCVSurfSupport.h"
#include "../../OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp"

class OpenCVGpuBFFMatcherWorkConsumer: public WorkConsumer {
public:
	OpenCVGpuBFFMatcherWorkConsumer();
	virtual ~OpenCVGpuBFFMatcherWorkConsumer();
	static const int surfThreshold = 13107200;
	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);
	static void * find_matches_thread(void * matches_args);
	uint32_t * keypointsmatches_to_byte(SURFKeyPointMatch * matches, int *num);
	cv::FlannBasedMatcher flannMatcher;
	std::vector< std::vector< cv::KeyPoint > >  model_keypoints;
	std::vector< cv::Mat > model_descriptors;
	int opensurf_model_count;
private:
	cv::gpu::BruteForceMatcher_GPU_base gpu_matcher;
	uint8_t mode;
	void configure();
};

#endif /* SURFWORKCONSUMER_SURFWORKCONSUMER_H_ */
