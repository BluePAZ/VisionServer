#ifndef __CAPITESTS_WORK_CONSUMER_H__
#define __CAPITESTS_WORK_CONSUMER_H__

#ifdef __WIN32
//#include "stdafx.h"
	#include <vld.h>
	#include <thread>
	#include <chrono>
#endif

#include <unistd.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include "WorkItem/WorkItem.hpp"
#include "WorkConsumer/WorkConsumer.hpp"
#include "WorkTypeDefs.hpp"


//#include "ConcurrentQueue/ConcurrentQueue.hpp"
//#include "../WorkItem/WorkItem.hpp"
class CAPITestsWorkConsumer: public WorkConsumer {
	public:
		CAPITestsWorkConsumer();
		virtual ~CAPITestsWorkConsumer();

		//Methods do be implemented by subclass
		//implement function to use on WorkItem
		virtual void init();
		virtual void runWork(WorkItem * w);
		virtual void deInit();
		virtual void parseParams(uint8_t * params, uint32_t paramsLength);
	private:

};

#endif

