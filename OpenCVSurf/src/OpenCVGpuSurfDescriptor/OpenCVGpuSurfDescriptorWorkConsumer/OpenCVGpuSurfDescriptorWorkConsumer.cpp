/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVGpuSurfDescriptorWorkConsumer.h"

OpenCVGpuSurfDescriptorWorkConsumer::OpenCVGpuSurfDescriptorWorkConsumer() {
	// TODO Auto-generated constructor stub

}
OpenCVGpuSurfDescriptorWorkConsumer::~OpenCVGpuSurfDescriptorWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void OpenCVGpuSurfDescriptorWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
}
void OpenCVGpuSurfDescriptorWorkConsumer::runWork(WorkItem * w){
	OpenCVSurfFullWorkItem * wi = (OpenCVSurfFullWorkItem *)w;
	cv::gpu::GpuMat gpuMatIn;
	cv::gpu::GpuMat gpu_keypoints, gpu_descriptors;
    cv::Mat mat = wi->image;
	std::vector< cv::KeyPoint > keypoints = wi->keypoints;
	gpu_surf = new cv::gpu::SURF_GPU(700, 4, 2, false, 0.01f, false);
	(*gpu_surf).uploadKeypoints(keypoints, gpu_keypoints);
	gpuMatIn.upload(mat);
	cv::gpu::cvtColor(gpuMatIn, gpuMatIn, CV_BGR2GRAY);
    std::vector<float> src_descriptors;
	//extract for unknown
	auto t1 = std::chrono::system_clock::now();
	(*gpu_surf)(gpuMatIn, cv::gpu::GpuMat(), gpu_keypoints, gpu_descriptors, true);
	(*gpu_surf).downloadDescriptors(gpu_descriptors, src_descriptors);
	auto t2 = std::chrono::system_clock::now();
		    	std::cout << "Surf GPU Descriptors: "
		    	  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		    	  << " milliseconds" << std::endl;
	wi->descriptors_matrix = cv::Mat(keypoints.size(),64,CV_32FC1, &src_descriptors[0]);
    (*gpu_surf).releaseMemory();
	gpuMatIn.release();
}
void OpenCVGpuSurfDescriptorWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVGpuSurfDescriptorWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVGpuSurfDescriptorWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}

