/*
 * SURFIPWorkTask.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef OPENCVOCLSURFIPDESCRIPTOR_WORKTASK_H
#define OPENCVOCLSURFIPDESCRIPTOR_WORKTASK_H
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <WorkTask/WorkTask.h>
#include <SocketData/SocketData.hpp>

class OpenCVGpuSurfIpDescriptorWorkTask: public WorkTask {
public:
	OpenCVGpuSurfIpDescriptorWorkTask();
	virtual ~OpenCVGpuSurfIpDescriptorWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new OpenCVGpuSurfIpDescriptorWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);

    /** Implement this method in your subclass with the code you want your thread to run. */
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
};

#endif /* CAPISURFIPTASK_CAPISURFIPTASK_H_ */
