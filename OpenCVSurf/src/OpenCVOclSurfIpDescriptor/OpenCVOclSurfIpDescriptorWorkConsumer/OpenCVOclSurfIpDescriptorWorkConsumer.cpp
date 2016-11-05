/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#if 0
#include "OpenCVOclSurfIpDescriptorWorkConsumer.h"

OpenCVOclSurfIpDescriptorWorkConsumer::OpenCVOclSurfIpDescriptorWorkConsumer() {
	// TODO Auto-generated constructor stub

}
OpenCVOclSurfIpDescriptorWorkConsumer::~OpenCVOclSurfIpDescriptorWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void OpenCVOclSurfIpDescriptorWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}
void OpenCVOclSurfIpDescriptorWorkConsumer::runWork(WorkItem * w){

	parseParams((uint8_t *)w->params, w->params_length);
	cv::ocl::oclMat graymat;
	cv::ocl::oclMat mat = cv::ocl::oclMat(currentHeight, currentWidth, CV_8UC3, (void *)w->data);
	//TODO:: implement man logic
	//Convert Mat to grey
	cv::ocl::cvtColor(mat, graymat, CV_BGR2GRAY);
	cv::ocl::SURF_OCL * ocl_surf = new cv::ocl::SURF_OCL(500, 5, 2, false, 0.01f, false);
	std::vector< cv::KeyPoint > * keypoints = new std::vector< cv::KeyPoint >();
	cv::ocl::oclMat * descriptors = new cv::ocl::oclMat();
	//extract for unknown
	(*ocl_surf)(graymat, cv::ocl::oclMat(), *keypoints, *descriptors, false);
	//cv::Mat  mm;
	//graymat.download(mm);
	w->result = (uint8_t *)keypoints;
	w->data = (uint8_t *)descriptors;
}
void OpenCVOclSurfIpDescriptorWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVOclSurfIpDescriptorWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVOclSurfIpDescriptorWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}
#endif

