/*
 * IntelProject2016DemoWorkTask.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#ifndef PersonDetectorPipelineWorkTask_H_
#define PersonDetectorPipelineWorkTask_H_

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

//Base needed for functionality
#include "WorkTypeDefs.hpp"
#include "SocketData/SocketData.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkItem/WorkItem.hpp"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"

#define mymax(x,y) ((x>=y)? x : y)
#define mymin(x,y) ((x>=y)? y : x)
class PersonDetectorPipelineWorkTask : public WorkTask {
public:
	PersonDetectorPipelineWorkTask();
	virtual ~PersonDetectorPipelineWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new PersonDetectorPipelineWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
	double const hwScale = 1.1;
	void nonMaxSurpression(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps);


	enum HARDWAREHOGPARAMS : uint32_t
	{
		KERNELWIDTH = 3,
		KERNELHEIGHT = 3,
		NUMLAYERS = 15,
		BASESCALE = 1,
		SVMROIWIDTH = 14,
		SVMROICHANNELS = 31,
		CELLSIZE = 8,
		ROIPIXWIDTH = 64,
		ROIPIXHEIGHT = 128,
		DETECTIONTHRESHOLD = 122000,
		BITSPERSVMBIN = 32,
		NUMSVMBIN = 18,
		NUMRHABIN = 32,
		RHA_NCA_SCALEFACTOR = 64,
		RHABATCHSIZE = 256 // 128 histograms are stored by RHA in one batch
	};
private:
	std::vector<std::string> parseConfig(uint8_t * data, uint32_t length);
	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
};

#endif
