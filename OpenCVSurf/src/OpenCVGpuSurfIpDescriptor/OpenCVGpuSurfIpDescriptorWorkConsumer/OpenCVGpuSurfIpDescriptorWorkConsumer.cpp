/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVGpuSurfIpDescriptorWorkConsumer.h"

OpenCVGpuSurfIpDescriptorWorkConsumer::OpenCVGpuSurfIpDescriptorWorkConsumer() {
	// TODO Auto-generated constructor stub
}
OpenCVGpuSurfIpDescriptorWorkConsumer::~OpenCVGpuSurfIpDescriptorWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void OpenCVGpuSurfIpDescriptorWorkConsumer::init(){
}
void OpenCVGpuSurfIpDescriptorWorkConsumer::runWork(WorkItem * w){
	OpenCVSurfFullWorkItem * wi = (OpenCVSurfFullWorkItem *)w;
	if(wi->is_cpu == false){
		cv::gpu::GpuMat gpuMatIn;
		cv::gpu::GpuMat gpuMask;
		cv::gpu::SURF_GPU surfgpu(MIN_HESSIAN);
		//gpu_surf = new cv::gpu::SURF_GPU(400, 4, 2, false, 0.01f, false);
		//fprintf(stderr, "Width:%u, Height: %u, Data: %x\n", mat.cols, mat.rows, mat.data);
		gpuMatIn.upload(wi->image);
		gpuMask.upload(wi->mask);
		//cv::gpu::cvtColor(gpuMatIn, gpuMatIn, CV_BGR2GRAY);
		std::vector< cv::KeyPoint > keypoints;
		std::vector<float> src_descriptors;
		cv::gpu::GpuMat gpu_keypoints, gpu_descriptors;
		auto t1 = std::chrono::system_clock::now();
		surfgpu(gpuMatIn, gpuMask , gpu_keypoints, gpu_descriptors, false);
		surfgpu.downloadDescriptors(gpu_descriptors, src_descriptors);
		surfgpu.downloadKeypoints(gpu_keypoints, keypoints);
		auto t2 = std::chrono::system_clock::now();
					std::cout << "Surf Full GPU: "
					  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
					  << " milliseconds" << std::endl;
		fprintf(stderr, "# of Keypoints: %u\n", keypoints.size());
		wi->descriptors_matrix = cv::Mat(keypoints.size(),64,CV_32FC1, &src_descriptors[0]);
		wi->keypoints = keypoints;
		surfgpu.releaseMemory();
	}else{
		fprintf(stderr, "Falling back to cpu\n");
	    cv::SURF * surf = new cv::SURF(MIN_HESSIAN, 5, 2, false, false);
		cv::Mat mat = wi->image;
		cv::Mat greymat;
		if(mat.channels() != 1){
			cv::cvtColor(mat, greymat, CV_BGR2GRAY);
		}else{
			greymat = mat;
		}
		std::vector< cv::KeyPoint > keypoints;
		cv::Mat descriptors;	//extract for unknown
		(*surf)(greymat, cv::noArray(), keypoints, descriptors, false);
		((OpenCVSurfFullWorkItem *)wi)->descriptors_matrix = descriptors;
		((OpenCVSurfFullWorkItem *)wi)->keypoints = keypoints;
		delete surf;
	}
    //(*gpu_surf).releaseMemory();
}
void OpenCVGpuSurfIpDescriptorWorkConsumer::deInit(){
	//TODO:: implement cleanup
}
void OpenCVGpuSurfIpDescriptorWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void OpenCVGpuSurfIpDescriptorWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}

