/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfDescriptorWorkConsumer.h"

OpenCVSurfDescriptorWorkConsumer::OpenCVSurfDescriptorWorkConsumer() {
	// TODO Auto-generated constructor stub
    surf = new cv::SURF(MIN_HESSIAN, 4, 2, false, false);
}
OpenCVSurfDescriptorWorkConsumer::~OpenCVSurfDescriptorWorkConsumer() {
	// TODO Auto-generated destructor stub
    delete surf;
}
void OpenCVSurfDescriptorWorkConsumer::init(){
}
void OpenCVSurfDescriptorWorkConsumer::runWork(WorkItem * w){
	OpenCVSurfFullWorkItem * data_item = (OpenCVSurfFullWorkItem *)w;
    cv::Mat greymat = data_item->image;
	std::vector< cv::KeyPoint > keypoints = data_item->keypoints;
    cv::Mat descriptors;
	auto t1 = std::chrono::system_clock::now();
	(*surf)(greymat, cv::noArray(), keypoints, descriptors, true);
	auto t2 = std::chrono::system_clock::now();
			std::cout << "Surf DESCRIPTOR: "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
			  << " milliseconds" << std::endl;
    ((OpenCVSurfFullWorkItem *)w)->descriptors_matrix = descriptors;
}
cv::Mat OpenCVSurfDescriptorWorkConsumer::aggregate_descriptors(struct desc_args * args){
	cv::Mat agg;
	for(int j = 0; j<NUM_THREADS; j++){
		agg.push_back(*(args[j].thread_descriptor));
	}
	return agg;
}
std::vector< std::vector< cv::KeyPoint> *> * OpenCVSurfDescriptorWorkConsumer::split_keypoints(std::vector<cv::KeyPoint> * keypoints){
	int length = keypoints->size();
	int perThread = length / NUM_THREADS;
	int lastThread = perThread + (length % NUM_THREADS);
	std::vector< std::vector<cv::KeyPoint> *> * returnVector = new std::vector< std::vector<cv::KeyPoint> *>();
	for(int i = 0; i<NUM_THREADS; i++){
		std::vector<cv::KeyPoint> * thread_vector = new std::vector<cv::KeyPoint>();
		if( i != NUM_THREADS - 1){
			for(int j = 0; j < perThread; j++){
				thread_vector->push_back((*keypoints)[i * perThread + j]);
			}
		}else{
			for(int j = 0; j < lastThread; j++){
				thread_vector->push_back((*keypoints)[i * lastThread + j]);
			}
		}
		returnVector->push_back(thread_vector);
	}
	return returnVector;
}
void * OpenCVSurfDescriptorWorkConsumer::thread_descriptor(void * args){
		const cv::Mat image_mat = *((struct desc_args *)args)->thread_image;
		cv::SURF * surf = new cv::SURF(300, 4, 2, false, false);
		//cv::SurfDescriptorExtractor detector;
		//detector.compute(image_mat, *((struct desc_args *)args)->keypoints, *((struct desc_args *)args)->thread_descriptor);
		((struct desc_args *)args)->thread_descriptor = new cv::Mat();
		(*surf)(image_mat, cv::noArray(), *((struct desc_args *)args)->keypoints, *((struct desc_args *)args)->thread_descriptor, true);
		pthread_exit(0);
}
void OpenCVSurfDescriptorWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVSurfDescriptorWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVSurfDescriptorWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}



