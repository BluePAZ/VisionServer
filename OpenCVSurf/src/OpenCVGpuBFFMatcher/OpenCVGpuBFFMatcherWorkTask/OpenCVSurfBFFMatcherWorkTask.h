/*
 * SURFIPWorkTask.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */
#ifndef __OPENCVSURFBFFMATCHER_WORKTASK_H
#define __OPENCVSURFBFFMATCHER_WORKTASK_H
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <WorkTask/WorkTask.h>
#include <SocketData/SocketData.hpp>

class OpenCVSurfBFFMatcherWorkTask: public WorkTask {
public:
	OpenCVSurfBFFMatcherWorkTask();
	virtual ~OpenCVSurfBFFMatcherWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new OpenCVSurfBFFMatcherWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
    void runTaskThreadWork(struct WorkTaskThreadArgs * arg);
	//virtual void packetReceived(SocketData * data);
};

#endif /* CAPISURFIPTASK_CAPISURFIPTASK_H_ */
