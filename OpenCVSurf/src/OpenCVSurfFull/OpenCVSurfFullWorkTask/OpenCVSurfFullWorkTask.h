/*
 * SURFIPWorkTask.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef OPENCVSURFIPDESCRIPTOR_WORKTASK_H
#define OPENCVSURFIPDESCRIPTOR_WORKTASK_H
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>


#include <WorkTask/WorkTask.h>
#include <SocketData/SocketData.hpp>

typedef std::chrono::high_resolution_clock Clock;

class OpenCVSurfFullWorkTask: public WorkTask {
public:
	OpenCVSurfFullWorkTask();
	virtual ~OpenCVSurfFullWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new OpenCVSurfFullWorkTask(); };
	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	void setParams(WorkItem * i, uint32_t width, uint32_t height, uint32_t depth=3);
	//virtual void packetReceived(SocketData * data);
	/** Implement this method in your subclass with the code you want your thread to run. */
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
	private:
};

#endif /* CAPISURFIPTASK_CAPISURFIPTASK_H_ */
