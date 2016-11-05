#ifndef __DATA_MANAGER_WORK_CONSUMER_HPP__
#define __DATA_MANAGER_WORK_CONSUMER_HPP__
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

#include "WorkItem/WorkItem.hpp"
#include "WorkConsumer/WorkConsumer.hpp"
#include "WorkTypeDefs.hpp"

//#include "ConcurrentQueue/ConcurrentQueue.hpp"
//#include "../WorkItem/WorkItem.hpp"
class DataManagerWorkConsumer: public WorkConsumer {
	public:
		DataManagerWorkConsumer();
		virtual ~DataManagerWorkConsumer();
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

#endif //__DATA_MANAGER_WORK_CONSUMER_HPP__

