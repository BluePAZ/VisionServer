/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfIpDescriptorWorkConsumer.h"

OpenCVSurfIpDescriptorWorkConsumer::OpenCVSurfIpDescriptorWorkConsumer() {
	// TODO Auto-generated constructor stub
    surf = new cv::SURF(MIN_HESSIAN, 5, 2, false, false);
}
OpenCVSurfIpDescriptorWorkConsumer::~OpenCVSurfIpDescriptorWorkConsumer() {
	// TODO Auto-generated destructor stub
    delete surf;
}
void OpenCVSurfIpDescriptorWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}
void OpenCVSurfIpDescriptorWorkConsumer::runWork(WorkItem * w){
	OpenCVSurfFullWorkItem * data_item = (OpenCVSurfFullWorkItem *)w;
    cv::Mat mat = data_item->image;
	cv::Mat greymat;
	if(mat.channels() != 1){
		cv::cvtColor(mat, greymat, CV_BGR2GRAY);
	}else{
		greymat = mat;
	}
    std::vector< cv::KeyPoint > keypoints;
    cv::Mat descriptors;	//extract for unknown
	(*surf)(greymat, cv::noArray(), keypoints, descriptors, false);
    ((OpenCVSurfFullWorkItem *)w)->descriptors_matrix = descriptors;
    ((OpenCVSurfFullWorkItem *)w)->keypoints = keypoints;
}
void OpenCVSurfIpDescriptorWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVSurfIpDescriptorWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVSurfIpDescriptorWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}



