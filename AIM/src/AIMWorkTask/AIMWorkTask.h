/*
 * CAPISaliencyTask.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */

#ifndef AIMWORKTASK_H_
#define AIMWORKTASK_H_

#include <OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp>
#include "opencv2/opencv.hpp" //use opencv
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "SocketData/SocketData.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkItem/WorkItem.hpp"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"

#include "OpenCVSurfSupport/OpenCVSurfSupport.h"


class AIMWorkTask : public WorkTask {
public:
	AIMWorkTask();
	virtual ~AIMWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();
	static WorkTask * Create() { return new AIMWorkTask(); };
	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);
	virtual void runTaskThreadWork(struct WorkTaskThreadArgs *);

};

#endif /* CAPISALIENCYTASK_H_ */
