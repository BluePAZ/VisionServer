/*
 * SURFIPWorkTask.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef __OPENCVSURFFULLDEMO_H__
#define __OPENCVSURFFULLDEMO_H__
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
#include "../../OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp"
#include "../../OpenCVSurfSupport/OpenCVSurfSupport.h"

/* for opencv rect not sure which one is needed */
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include <unordered_map>


typedef std::chrono::high_resolution_clock Clock;

class OpenCVSurfFullDemoWorkTask: public WorkTask {
public:
	OpenCVSurfFullDemoWorkTask();
	virtual ~OpenCVSurfFullDemoWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new OpenCVSurfFullDemoWorkTask(); };
	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	void setParams(WorkItem * i, uint32_t width, uint32_t height, uint32_t depth=3);
	//virtual void packetReceived(SocketData * data);
	/** Implement this method in your subclass with the code you want your thread to run. */
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
	private:
	std::unordered_map<uint32_t, OpenCVSurfFullWorkItem *> model_map;
};

#endif /* CAPISURFIPTASK_CAPISURFIPTASK_H_ */
