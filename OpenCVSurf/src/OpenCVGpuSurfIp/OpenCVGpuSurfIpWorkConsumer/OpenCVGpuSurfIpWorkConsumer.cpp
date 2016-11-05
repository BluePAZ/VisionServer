/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVGpuSurfIpWorkConsumer.h"

OpenCVGpuSurfIpWorkConsumer::OpenCVGpuSurfIpWorkConsumer() {
	// TODO Auto-generated constructor stub
    gpu_surf = new cv::gpu::SURF_GPU(500, 5, 2, false, 0.01f, false);
}
OpenCVGpuSurfIpWorkConsumer::~OpenCVGpuSurfIpWorkConsumer() {
	// TODO Auto-generated destructor stub
    delete gpu_surf;
}
void OpenCVGpuSurfIpWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
}
void OpenCVGpuSurfIpWorkConsumer::runWork(OpenCVSurfWorkItem * w){
    OpenCVSurfWorkItem * data_item = (OpenCVSurfWorkItem *)w;
	cv::gpu::GpuMat gpuMatIn;
	cv::gpu::GpuMat gpu_keypoints;
	cv::gpu::GpuMat gpuMatOut;
	cv::Mat mat = data_item->image;
    std::vector< cv::KeyPoint > keypoints;
    //(*gpu_surf).uploadKeypoints(*keypoints, gpu_keypoints);
	gpuMatIn.upload(mat);
    std::vector<float> src_descriptors;
	//extract for unknown
	auto t1 = std::chrono::system_clock::now();
	(*gpu_surf)(gpuMatIn, cv::gpu::GpuMat(), gpu_keypoints, gpu_descriptors, true);
	(*gpu_surf).downloadKeypoints(gpu_keypoints, keypoints);
	auto t2 = std::chrono::system_clock::now();
		    	std::cout << "Surf IP GPU: "
		    	  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		    	  << " milliseconds" << std::endl;
	(*gpu_surf).releaseMemory();
    w->keypoints = keypoints;
}
void OpenCVGpuSurfIpWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVGpuSurfIpWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVGpuSurfIpWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}

