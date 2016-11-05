/*
 * IntelProject2016DemoWorkTask.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#ifndef IntelProject2016DemoWorkTask_H_
#define IntelProject2016DemoWorkTask_H_

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

/* for opencv rect not sure which one is needed */
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

//Needed for H264 Decode
#include "CAPIFrameDecoder/CAPIFrameDecoder.hpp"

//Base needed for functionality
#include "WorkTypeDefs.hpp"
#include "SocketData/SocketData.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkItem/WorkItem.hpp"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"

#include "OpenCVSurfSupport/OpenCVSurfSupport.h"

class IntelProject2016DemoWorkTask : public WorkTask {
public:
	IntelProject2016DemoWorkTask();
	virtual ~IntelProject2016DemoWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new IntelProject2016DemoWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
private:
	CAPIFrameDecoder * h264_decoder;
	std::vector<std::string> parseConfig(uint8_t * data, uint32_t length);
	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
};

#endif /* CAPISALIENCYTASK_H_ */
