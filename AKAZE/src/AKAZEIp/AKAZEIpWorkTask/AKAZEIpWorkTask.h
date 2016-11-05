/*
 * SURFIPWorkTask.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef AKAZEIP_WORKTASK_H
#define AKAZEIP_WORKTASK_H
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <WorkTask/WorkTask.h>
#include <SocketData/SocketData.hpp>
#include "../../AKAZESupport/AKAZESupport.h"

class AKAZEIpWorkTask: public WorkTask {
public:
	AKAZEIpWorkTask();
	virtual ~AKAZEIpWorkTask();
	std::string getTaskName();
	static std::string returnTaskName();
	static WorkTask * Create() { return new AKAZEIpWorkTask(); };
	virtual void init();
	virtual void runTask(SocketData * data);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	void setParams(WorkItem * i, uint32_t width, uint32_t height, uint32_t depth=3);

	//virtual void packetReceived(SocketData * data);
    /** Implement this method in your subclass with the code you want your thread to run. */
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
};

#endif /* CAPISURFIPTASK_CAPISURFIPTASK_H_ */
