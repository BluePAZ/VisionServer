/*
 * CAPISaliencyTask.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#ifndef CAPITESTSWORKTASK_H_
#define CAPITESTSWORKTASK_H_

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

#include "WorkTypeDefs.hpp"
#include "SocketData/SocketData.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkItem/WorkItem.hpp"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"

class CAPITestsWorkTask : public WorkTask {
public:
	CAPITestsWorkTask();
	virtual ~CAPITestsWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new CAPITestsWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
private:
};

#endif /* CAPISALIENCYTASK_H_ */
