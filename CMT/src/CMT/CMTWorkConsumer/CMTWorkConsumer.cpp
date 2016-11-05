/*
 * CMTWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "CMTWorkConsumer.h"

CMTWorkConsumer::CMTWorkConsumer() {
	// TODO Auto-generated constructor stub


}
CMTWorkConsumer::~CMTWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void CMTWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}
void CMTWorkConsumer::runWork(WorkItem * w){
	//TODO:: implement man logic
	parseParams((uint8_t *)w->params, w->params_length);
	//Convert Mat to grey
	//cv::Mat greymat;
	fprintf(stderr, "Width: %i, HEight %i\n", currentWidth, currentHeight);
	///cv::Mat mat = cv::Mat(currentHeight, currentWidth, CV_8U, (void *)w->data);
	//cv::cvtColor(mat, greymat, CV_BGR2GRAY);
	//cv::resize(greymat, greymat, cv::Size(1024,768),0,0, CV_INTER_CUBIC);
	auto t1 = std::chrono::system_clock::now();


	auto t2 = std::chrono::system_clock::now();
	    	std::cout << "Surf IP: "
	    	  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
	    	  << " milliseconds" << std::endl;
	//w->setResults((uint8_t *)keypoints, 1, false, true, true, true);
}
void CMTWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void CMTWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void CMTWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}
