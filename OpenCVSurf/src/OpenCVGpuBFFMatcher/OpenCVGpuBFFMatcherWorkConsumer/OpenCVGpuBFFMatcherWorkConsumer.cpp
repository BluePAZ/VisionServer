/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVGpuBFFMatcherWorkConsumer.h"

OpenCVGpuBFFMatcherWorkConsumer::OpenCVGpuBFFMatcherWorkConsumer() {
	// TODO Auto-generated constructor stub

}
OpenCVGpuBFFMatcherWorkConsumer::~OpenCVGpuBFFMatcherWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void OpenCVGpuBFFMatcherWorkConsumer::init(){
	cv::gpu::setDevice(0);
}
void OpenCVGpuBFFMatcherWorkConsumer::runWork(WorkItem * w){
    fprintf(stderr, "Starting GPU\n");
	//TODO:: implement man logic
    OpenCVSurfFullWorkItem * wi =(OpenCVSurfFullWorkItem *)w;
	if(wi->is_cpu == false){
		cv::gpu::GpuMat descriptors1GPU, descriptors2GPU;
		//cv::Mat * matrix = new cv::Mat(((std::vector<cv::KeyPoint> *)w->result)->size(),64,CV_32FC1,&((std::vector<float> *)w->data)[0]);
		descriptors1GPU.upload((wi->descriptors_matrix));
		descriptors2GPU.upload((wi->model_descriptors_matrix));
		//std::vector<std::vector<cv::DMatch>> matches;
		cv::gpu::GpuMat trainIdx, distance;
		auto t1 = std::chrono::system_clock::now();
		gpu_matcher.knnMatch(descriptors1GPU, descriptors2GPU, wi->matches, 2);
		auto t2 = std::chrono::system_clock::now();
					std::cout << "Surf GPU Matching: "
					  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
					  << " milliseconds" << std::endl;
		descriptors1GPU.release();
		descriptors2GPU.release();
	}
    fprintf(stderr, "# of Matches: %u\n", wi->matches.size());
}
void OpenCVGpuBFFMatcherWorkConsumer::deInit(){

}
void OpenCVGpuBFFMatcherWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	mode = paramsLength;
	configure();
}
void OpenCVGpuBFFMatcherWorkConsumer::configure(){
}

uint32_t * OpenCVGpuBFFMatcherWorkConsumer::keypointsmatches_to_byte(SURFKeyPointMatch * matches, int *num){
	int count = *num;
	*num = *num * 4 * (10);
	uint32_t * data = (uint32_t *)malloc(*num);
	for(int i = 0; i < count; ++i){
		int idx = i*(10);
		data[idx + 0] = *((uint32_t *)(&(matches[i].model_id)));
		data[idx + 1] = *((uint32_t *)(&(matches[i].model_x)));
		data[idx + 2] = *((uint32_t *)(&(matches[i].model_y)));
		memcpy(&(data[idx + 3]), &(matches[i].model_size.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 4]), &(matches[i].model_angle.bytes[0]), sizeof(float));
		data[idx + 5] = *((uint32_t *)(&(matches[i].frame_x)));
		data[idx + 6] = *((uint32_t *)(&(matches[i].frame_y)));
		memcpy(&(data[idx + 7]), &(matches[i].frame_size.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 8]), &(matches[i].frame_angle.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 9]), &(matches[i].model_score.bytes[0]), sizeof(float));
	}
	free(matches);
	return data;
}


