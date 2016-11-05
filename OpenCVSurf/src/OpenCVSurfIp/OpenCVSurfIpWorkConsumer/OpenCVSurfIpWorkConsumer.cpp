/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfIpWorkConsumer.h"

OpenCVSurfIpWorkConsumer::OpenCVSurfIpWorkConsumer() {
	// TODO Auto-generated constructor stub
    surf = new cv::SURF(MIN_HESSIAN, 4, 2, false, false);
}
OpenCVSurfIpWorkConsumer::~OpenCVSurfIpWorkConsumer() {
	// TODO Auto-generated destructor stub
    delete surf;

}
void OpenCVSurfIpWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}
void OpenCVSurfIpWorkConsumer::runWork(WorkItem * w){
	//TODO:: implement man logic
	OpenCVSurfFullWorkItem * data_item = (OpenCVSurfFullWorkItem *)w;
    cv::Mat mat = data_item->image;
    cv::Mat greymat;
    if(mat.channels() != 1){
    	cv::cvtColor(mat, greymat, CV_BGR2GRAY);
    }else{
    	greymat = mat;
    }
    std::vector< cv::KeyPoint > keypoints;
    //extract for unknown
	auto t1 = std::chrono::system_clock::now();
	(*surf)(mat, cv::noArray(), keypoints, cv::noArray(), false);
	auto t2 = std::chrono::system_clock::now();
	    	std::cout << "Surf IP CPU: "
	    	  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
	    	  << " milliseconds" << std::endl;
    ((OpenCVSurfFullWorkItem *)w)->image = greymat;
    ((OpenCVSurfFullWorkItem *)w)->keypoints = keypoints;
}
void OpenCVSurfIpWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVSurfIpWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVSurfIpWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}
