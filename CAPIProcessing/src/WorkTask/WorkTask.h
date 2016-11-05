/*
 * WorkTask.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */

#ifndef WORKTASK_WORKTASK_H_
#define WORKTASK_WORKTASK_H_
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <string>
#include <unordered_map>
#include <pthread.h>

#include "CAPINetworkUtility/CAPINetworkUtility.hpp"
#include "../WorkManager/WorkManager.hpp"
#include "../WorkManagerDispatcher/WorkManagerDispatcher.h"
#include "../WorkItem/WorkItem.hpp"
#include "CAPINetworkDefs.hpp"
#include "../WorkTypeDefs.hpp"
#include "../DataSharer/DataSharer.hpp"

class WorkTask;

struct WorkTaskThreadArgs{
	WorkTask * caller;
	WorkItem * item;
};

class WorkTask {
public:
	WorkTask();
	virtual ~WorkTask();

	uint8_t busy;
	uint8_t running;
	uint8_t complete;
	/*
	 * Pointer to the different shared work queue
	 */
	WorkManagerDispatcher * dispatcher;
	/*
	 * Functions to manage own connection
	 */
	void registerSocket(int socket, int stream_id);
	CAPINetworkUtility * network;
	DataSharer * shared_data;
	int socket_stream_id;
	/*
	 * Functions to identify task
	 */
	//static const std::string getName(){ return taskName; }
	virtual std::string getTaskName()=0;
	/*
	 * Functions for task
	 */
	void startTask();
	void stopTask();
	virtual void init()=0;
	virtual void runTask(SocketData * data_in)=0;
	virtual void deInit()=0;
	virtual void parseParams(uint8_t * params)=0;
	void packetReceived(SocketData * data);
	//void * packetReceivingThread(void *);
	void sendDataPacket(SocketData * data);
	/** Returns true if the thread was successfully started, false if there was an error starting the thread */
	int StartInternalThread()
	{
		return pthread_create(&_thread, NULL, InternalThreadEntryFunc, this);
	}
	/** Will not return until the internal thread has exited. */
	void WaitForInternalThreadToExit()
	{
		(void) pthread_join(_thread, NULL);
	}
	/** Implement this method in your subclass with the code you want your thread to run. */
	void InternalThreadEntry();
	virtual void runTaskThreadWork(struct WorkTaskThreadArgs *) = 0;
	static void * RunTaskThreadEntry(void * args){
		struct WorkTaskThreadArgs * arg = ((struct WorkTaskThreadArgs *)(args));
		arg->caller->runTaskThreadWork(arg);
		pthread_exit(0);
		return NULL;
	}
	bool isSingleThreaded;

private:
	pthread_t _thread;
	//void startReceiving();
	static void * InternalThreadEntryFunc(void * This) {((WorkTask *)This)->InternalThreadEntry(); return NULL;}

};
//type def at the end for type
typedef WorkTask*  (*taskCreateFunction)(void);


#endif /* WORKTASK_WORKTASK_H_ */
