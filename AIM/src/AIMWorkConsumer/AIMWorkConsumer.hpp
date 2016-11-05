#ifndef __AIM_WORK_CONSUMER_H__
#define __AIM_WORK_CONSUMER_H__
#ifdef __WIN32
//#include "stdafx.h"
	#include <vld.h>
	#include <thread>
	#include <chrono>
#endif
//#include "nifsap_logger_v1_00_a.h"
#include "nif_capi_v2_00_b.h"
#include "aim_engine_v2_00_b.h"
#include "aim_engine_v2_00_b_gabor_kernel.h"
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

#include "WorkItem/WorkItem.hpp"
#include "WorkConsumer/WorkConsumer.hpp"
#include "WorkTypeDefs.hpp"


//#include "ConcurrentQueue/ConcurrentQueue.hpp"
//#include "../WorkItem/WorkItem.hpp"
class AIMWorkConsumer: public WorkConsumer {
	public:
		AIMWorkConsumer();
		AIMWorkConsumer(IVortexNIFSAP *accel);
		virtual ~AIMWorkConsumer();


		IVortexNIFSAP * AcceleratorInterface;
		aim_engine_v2_00_b * AIMEngine;
		VortexMemoryAllocation * ImageHandle;

		//Methods do be implemented by subclass
		//implement function to use on WorkItem
		virtual void init();
		virtual void runWork(WorkItem * w);
		virtual void deInit();
		virtual void parseParams(uint8_t * params, uint32_t paramsLength);
		void configure();

	private:
		int newWidth = 192;
		int newHeight = 192;
		int newDepth = 4;
		int currentWidth = 192;
		int currentHeight = 192;
		int currentDepth = 0;
		//possibly not needed passsed by param
		int dataSize = 0;
};

#endif

