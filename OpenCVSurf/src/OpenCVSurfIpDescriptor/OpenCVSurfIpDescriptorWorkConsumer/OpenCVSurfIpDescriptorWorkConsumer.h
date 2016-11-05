/*
 *network SURFWorkConsumer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef OPENCVSURFIPDESCRIPTOR_WORKCONSUMER_H
#define OPENCVSURFIPDESCRIPTOR_WORKCONSUMER_H
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


class OpenCVSurfIpDescriptorWorkConsumer: public WorkConsumer {
public:
	OpenCVSurfIpDescriptorWorkConsumer();
	virtual ~OpenCVSurfIpDescriptorWorkConsumer();

	static const int surfThreshold = 13107200;

	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);
private:
    cv::SURF * surf;
	void configure();
	int newWidth = 1920;
	int newHeight = 1200;
	int newDepth = 4;
	int currentWidth = 1920;
	int currentHeight = 1200;
	int currentDepth = 4;
	//possibly not needed passsed by param
	int dataSize = 0;
};

#endif /* SURFWORKCONSUMER_SURFWORKCONSUMER_H_ */
