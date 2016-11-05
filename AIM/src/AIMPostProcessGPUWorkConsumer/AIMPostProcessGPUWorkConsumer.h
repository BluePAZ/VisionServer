/*
 * AIMPostProcessGPUWorkConsumer.h
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

#ifndef SRC_AIMPOSTPROCESSGPUWORKCONSUMER_AIMPOSTPROCESSWORKCONSUMER_H_
#define SRC_AIMPOSTPROCESSGPUWORKCONSUMER_AIMPOSTPROCESSWORKCONSUMER_H_


#include "opencv2/opencv.hpp" //use opencv
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/gpu/gpu.hpp"

#include <WorkConsumer/WorkConsumer.hpp>

class AIMPostProcessGPUWorkConsumer: public WorkConsumer {
public:
	AIMPostProcessGPUWorkConsumer();
	virtual ~AIMPostProcessGPUWorkConsumer();

	//Methods do be implemented by subclass
	//implement function to use on WorkItem
	virtual void init();
	virtual void runWork(WorkItem * w);
	virtual void deInit();
	virtual void parseParams(uint8_t * params, uint32_t paramsLength);
	void configure();
private:
	int newWidth = 192;
	int newHeight = 192;
	int newDepth = 4;
	int currentWidth = 192;
	int currentHeight = 192;
	int currentDepth = 0;
	//possibly not needed passsed by param
	int dataSize = 0;
	cv::Mat binaryMask;
	uint8_t * postProcess(uint8_t * rawResult, uint32_t * length);
	int percentileHistogram[65536];
	double calculatePercentile(const cv::Mat & infomap, double percentile);
	cv::Mat GetSalienceMask(const cv::Mat &infomap, double thresholdPercent, int ErosionSize, int ClosureSize);
	double calcPercentile(const cv::Mat &sequence, double percentile);
};

#endif /* SRC_AIMPOSTPROCESSWORKCONSUMER_AIMPOSTPROCESSWORKCONSUMER_H_ */
