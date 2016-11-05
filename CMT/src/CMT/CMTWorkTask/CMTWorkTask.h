/*
 * IntelProject2016DemoWorkTask.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#ifndef CMTWORKTASK_H__
#define CMTWORKTASK_H__

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

#include "../../CMT.h"

//Needed for H264 Decode
//#include "CAPIFrameDecoder/CAPIFrameDecoder.hpp"

//Base needed for functionality
#include "WorkTypeDefs.hpp"
#include "SocketData/SocketData.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkItem/WorkItem.hpp"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"

//#include "OpenCVSurfSupport/OpenCVSurfSupport.h"

class CMTWorkTask : public WorkTask {
public:
	CMTWorkTask();
	virtual ~CMTWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new CMTWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data_in);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);
	void runTaskThreadWork(struct WorkTaskThreadArgs *);
private:


	bool trackerInitialized = false;
	bool running = true;


	cmt::CMT cmt_tracker;
	/**
	 * Fields needed for initial object detection with SURF
	 */
	int minHessian = 400;
	cv::SurfFeatureDetector detector;
	cv::SurfDescriptorExtractor extractor;

	// If there's no tracker yet, should we be attempting object detection?
	bool attemptObjectDetection = false;

	// The model image, keypoints, and descriptors of the object to detect
	cv::Mat object_image;
	cv::vector<cv::KeyPoint> object_keypoints;
	cv::Mat object_descriptors;

	// Scene keypoints and descriptors
	std::vector<cv::KeyPoint> scene_keypoints;
	cv::Mat scene_descriptors;

	double lastArea = -1.0f;
	cv::Point2d lastLocation = cv::Point2d(-1, -1);
	double scaleDelta = 1.0f;
	cv::Point2f positionDelta = cv::Point2f(1.0f, 1.0f);
	double scaleDeltaThreshold = 0.1f;
	cv::Point2f positionDeltaThreshold = cv::Point2f(0.1f, 0.1f);

	void initializeTracker(cv::Mat im_gray);
	void setObjectToFind(cv::Mat image);
	void processFrame(cv::Mat image);
	void changeModel(cv::Mat image);

	//CAPIFrameDecoder * h264_decoder;
	std::vector<std::string> parseConfig(uint8_t * data, uint32_t length);
	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
};

#endif /* CAPISALIENCYTASK_H_ */
