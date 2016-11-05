/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "AKAZEIpWorkConsumer.h"

AKAZEIpWorkConsumer::AKAZEIpWorkConsumer() {
	// TODO Auto-generated constructor stub

}
AKAZEIpWorkConsumer::~AKAZEIpWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void AKAZEIpWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}
void AKAZEIpWorkConsumer::runWork(WorkItem * w){
	//TODO:: implement man logic
	parseParams((uint8_t *)w->params, w->params_length);
	//w->freeParameters();
	//Convert Mat to grey
	//cv::Mat greymat;
	AKAZEOptions options;
	cv::Mat mat = cv::Mat(currentHeight, currentWidth, CV_8UC4, (void *)w->data);
	//cv::cvtColor(mat, greymat, CV_BGR2GRAY);
	//cv::resize(greymat, greymat, cv::Size(1024,768),0,0, CV_INTER_CUBIC);

	// Convert the image to float to extract features
	cv::Mat img_32;
	mat.convertTo(img_32, CV_32F, 1.0/255.0, 0);
	// Don't forget to specify image dimensions in AKAZE's options
	options.img_width = mat.cols;
	options.img_height = mat.rows;
	// Extract features
	libAKAZE::AKAZE evolution(options);
	std::vector< cv::KeyPoint > * kpts = new std::vector< cv::KeyPoint >();
	auto t1 = std::chrono::system_clock::now();
	evolution.Create_Nonlinear_Scale_Space(img_32);
	evolution.Feature_Detection(*kpts);
	//extract for unknown
	auto t2 = std::chrono::system_clock::now();
		std::cout << "AKAZE IP"
		  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		  << " milliseconds" << std::endl;

	w->setResults((uint8_t *)kpts, 1, false, true, true, true);
}
void AKAZEIpWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void AKAZEIpWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void AKAZEIpWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}
