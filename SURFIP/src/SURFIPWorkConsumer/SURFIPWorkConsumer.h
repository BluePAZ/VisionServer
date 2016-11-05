/*
 *network SURFWorkConsumer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef SURFWORKCONSUMER_SURFWORKCONSUMER_H_
#define SURFWORKCONSUMER_SURFWORKCONSUMER_H_


#ifdef __WIN32
//#include "stdafx.h"
	#include <vld.h>
	#include <thread>
	#include <chrono>
#endif
//#include "nifsap_logger_v1_00_a.h"
#include "nif_capi_v2_00_b.h"
#include "surf_ip_detector_v1_00_b.h"
#include "VortexSAPTypes.h"
#include "VortexTypes.h"
#include "VortexImageDescriptor.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include "../SURFIP_defs.h"

#include "WorkItem/WorkItem.hpp"
#include "WorkConsumer/WorkConsumer.hpp"
#include "WorkTypeDefs.hpp"


class SURFIPWorkConsumer: public WorkConsumer {
public:
	SURFIPWorkConsumer();
	virtual ~SURFIPWorkConsumer();

	static const int surfThreshold = 13107200;

	IVortexNIFSAP * AcceleratorInterface;
	surf_ip_detector_v1_00_b * SURFIPEngine;
	VortexMemoryAllocation * ImageHandle;

	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);

private:

	void configure();
	hardware_ip_point * decode_keypoints(uint8_t * data, uint32_t length);
	int newWidth = 1920;
	int newHeight = 1080;
	int newDepth = 1;
	int newThreshold = 13107200;
	int currentWidth = 1920;
	int currentHeight = 1080;
	int currentDepth = 1;
	int currentThreshold = 13107200;

	//possibly not needed passsed by param
	int dataSize = 0;
};

#endif /* SURFWORKCONSUMER_SURFWORKCONSUMER_H_ */
