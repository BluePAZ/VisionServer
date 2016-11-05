/*
 *network SURFWorkConsumer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef __OPENCVSURFBFFMATCHER_WORKCONSUMER_H__
#define __OPENCVSURFBFFMATCHER_WORKCONSUMER_H__

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
#include "PointF.h"

#include "../../OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp"
#include "../../OpenCVSurfSupport/OpenCVSurfSupport.h"


#define MODEL_COUNT 1

typedef struct match_thread_args{
	cv::Mat * model_descriptors;
	cv::Mat * unknown_descriptors;
	std::vector < cv::KeyPoint > * unknown_keypoints;
	std::vector < cv::KeyPoint > * model_keypoints;
	HomographyMatches ** results;
	int * result_length;
	int thread_id;
}match_thread_args;
class OpenCVSurfBFFMatcherWorkConsumer: public WorkConsumer {
public:
	OpenCVSurfBFFMatcherWorkConsumer();
	virtual ~OpenCVSurfBFFMatcherWorkConsumer();
	static const int surfThreshold = 13107200;
	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);
	SURFKeyPointMatch * opencv_to_surfkeypointmatch_thread_agg(int * result_lengths, SURFKeyPointMatch ** matches, int * total_length);
	static void * find_matches_thread(void * matches_args);
	uint32_t * keypointsmatches_to_byte(SURFKeyPointMatch * matches, int *num);
	cv::BFMatcher bfMatcher;
	cv::FlannBasedMatcher flannMatcher;
	std::vector< std::vector< cv::KeyPoint > >  model_keypoints;
	std::vector< cv::Mat > model_descriptors;
	int opensurf_model_count;
	pthread_t  * match_threads;
private:
	uint8_t mode;
	void configure();
	void update_match_threads();
};

#endif /* SURFWORKCONSUMER_SURFWORKCONSUMER_H_ */
