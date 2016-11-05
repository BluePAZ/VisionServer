
#ifndef __WORK_CONSUMER_H__
#define __WORK_CONSUMER_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdint.h>
#include <pthread.h>

#include "../ConcurrentQueue/ConcurrentQueue.hpp"
#include "../WorkItem/WorkItem.hpp"
#include "../WorkTypeDefs.hpp"


class WorkConsumer{
	public:
		WorkConsumer();
		virtual ~WorkConsumer()=0; //marks as pure virtual
		ConcurrentQueue * inputQueue;
		uint8_t busy;
		uint8_t running;
		uint8_t complete;
		WorkItem * currentWork;
		void setWorkerID(uint32_t id);
		//function pointer to status change
		void (*workStatusChanged)(uint32_t a, uint8_t b);

		void startWork();
		void stopWork();
		
		//Methods do be implemented by subclass
		const char * workName;
		//implement function to use on WorkItem
		virtual void init()=0;
		virtual void runWork(WorkItem * w)=0;
		virtual void deInit()=0;
		virtual void parseParams(uint8_t * params, uint32_t paramsLength)=0;
		
		uint8_t isHardware();
		/** Returns true if the thread was successfully started, false if there was an error starting the thread */
		bool StartInternalThread()
		{
			return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
		}
		/** Will not return until the internal thread has exited. */
		void WaitForInternalThreadToExit()
		{
			(void) pthread_join(_thread, NULL);
		}
		/** Implement this method in your subclass with the code you want your thread to run. */
   		void InternalThreadEntry();
	private:
		uint32_t workerId;
		void workComplete();
		static void * InternalThreadEntryFunc(void * This) {((WorkConsumer *)This)->InternalThreadEntry(); return NULL;}
		pthread_t _thread;
		//set value to 1 if hardware else 0;
		uint8_t hardwareAcclerator;

};

#endif
